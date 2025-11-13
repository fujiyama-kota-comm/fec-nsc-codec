#include "nsc_decoder.h"
#include "nsc_encoder.h"
#include "trellis.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   NSC Decoder (Rate 1/2, terminated)
   - Soft-decision (LLR)
   - Hard-decision (0/1)
   Trellis-based Viterbi decoder (標準実装)
   ============================================================ */

extern int nsc_info_len; // 情報ビット長 K
extern int nsc_code_len; // 符号ビット長 N = 2*(K+tail)

/* ------------------------------------------------------------
   Soft-decision branch metric (1 シンボル分)
   出力ビット 0/1 → BPSK (+1 / -1) にマッピングして
   LLR と内積を取る形で定義
   ------------------------------------------------------------ */
static inline double branch_metric_soft_symbol(const double *LLR,
                                               int symbol_index, int out_bit0,
                                               int out_bit1) {
  double v = LLR[2 * symbol_index];
  double w = LLR[2 * symbol_index + 1];

  /* 0 → +1, 1 → -1 */
  double s0 = (out_bit0 == 0) ? +1.0 : -1.0;
  double s1 = (out_bit1 == 0) ? +1.0 : -1.0;

  /* 負の相関（小さいほど良い） */
  return -(s0 * v + s1 * w);
}

/* ------------------------------------------------------------
   Hard-decision branch metric (Hamming distance, 1 シンボル分)
   ------------------------------------------------------------ */
static inline int branch_metric_hard_symbol(const int *rx_bits,
                                            int symbol_index, int out_bit0,
                                            int out_bit1) {
  int v = rx_bits[2 * symbol_index];
  int w = rx_bits[2 * symbol_index + 1];

  return (v != out_bit0) + (w != out_bit1);
}

/* ============================================================
   Soft-decision Viterbi Decoder
   LLR:      受信 LLR (長さ N)
   info_hat: 推定情報ビット（長さ K）
   code_hat: 再符号化系列（Optional, NULL 可）
   ============================================================ */
void nsc_decode_r05_soft(const double *LLR, int *info_hat, int *code_hat) {
  int K = nsc_info_len;
  int N = nsc_code_len;
  int steps = N / 2; // シンボル数 = K + tail (2)

  /* -------------------------------------------
     パスメトリックとバックポインタ
     metric_prev/state: 時刻 i-1
     metric_curr/state: 時刻 i
     prev_state[i][s], prev_bit[i][s]
     ------------------------------------------- */
  double metric_prev[4];
  double metric_curr[4];

  int *prev_state = (int *)malloc(sizeof(int) * steps * 4);
  int *prev_bit = (int *)malloc(sizeof(int) * steps * 4);

  if (!prev_state || !prev_bit) {
    fprintf(stderr, "Memory allocation failed in decoder\n");
    free(prev_state);
    free(prev_bit);
    return;
  }

  /* -------------------------------------------
     初期メトリック（時刻 -1 に相当）
     終端付きなので、開始状態は必ず STATE_A
     ------------------------------------------- */
  for (int s = 0; s < 4; s++)
    metric_prev[s] = 1e30;
  metric_prev[STATE_A] = 0.0;

  /* =======================================================
     Forward recursion
     各時刻 i について，
       - すべての遷移 (prev_state, input_bit) を試し，
       - new_state に対する最良パスを更新
     ======================================================= */
  for (int i = 0; i < steps; i++) {

    /* 各状態のメトリックを一旦∞にリセット */
    for (int s = 0; s < 4; s++) {
      metric_curr[s] = 1e30;
    }

    for (int ps = 0; ps < 4; ps++) {
      if (metric_prev[ps] >= 1e29)
        continue; // 到達不能な状態はスキップ

      for (int b = 0; b < 2; b++) {

        int ns = nsc_next_state[ps][b];
        int out_bit0 = nsc_output_bits[ps][b][0];
        int out_bit1 = nsc_output_bits[ps][b][1];

        double bm = branch_metric_soft_symbol(LLR, i, out_bit0, out_bit1);
        double cand = metric_prev[ps] + bm;

        if (cand < metric_curr[ns]) {
          metric_curr[ns] = cand;
          prev_state[i * 4 + ns] = ps;
          prev_bit[i * 4 + ns] = b;
        }
      }
    }

    /* 次のステップへ */
    for (int s = 0; s < 4; s++)
      metric_prev[s] = metric_curr[s];
  }

  /* -------------------------------------------
     終端状態の決定
     テールビットで STATE_A に強制終端しているので
     本来は STATE_A 固定でよい
     （念のため最小メトリックも見ておく）
     ------------------------------------------- */
  int state = STATE_A;
  double best_final = metric_prev[state];
  for (int s = 0; s < 4; s++) {
    if (metric_prev[s] < best_final) {
      best_final = metric_prev[s];
      state = s;
    }
  }

  /* =======================================================
     Backward trace
     時刻 i に対応するビット prev_bit[i][state] を取り出し，
     i < K のときだけ info_hat[i] に書き込む
     （i >= K はテールビットなので破棄）
     ======================================================= */
  for (int i = steps - 1; i >= 0; i--) {
    int b = prev_bit[i * 4 + state];
    int ps = prev_state[i * 4 + state];

    if (i < K) {
      info_hat[i] = b;
    }

    state = ps;
  }

  /* Optional: 再符号化 */
  if (code_hat) {
    nsc_encode_r05(info_hat, code_hat);
  }

  free(prev_state);
  free(prev_bit);
}

/* ============================================================
   Hard-decision Viterbi Decoder
   rx_bits: 0/1 の受信ビット列 (長さ N)
   ============================================================ */
void nsc_decode_r05_hard(const int *rx_bits, int *info_hat, int *code_hat) {
  int K = nsc_info_len;
  int N = nsc_code_len;
  int steps = N / 2;

  int metric_prev[4];
  int metric_curr[4];

  int *prev_state = (int *)malloc(sizeof(int) * steps * 4);
  int *prev_bit = (int *)malloc(sizeof(int) * steps * 4);

  if (!prev_state || !prev_bit) {
    fprintf(stderr, "Memory allocation failed in decoder (hard)\n");
    free(prev_state);
    free(prev_bit);
    return;
  }

  /* 初期メトリック */
  for (int s = 0; s < 4; s++)
    metric_prev[s] = 1000000000;
  metric_prev[STATE_A] = 0;

  /* Forward recursion */
  for (int i = 0; i < steps; i++) {

    for (int s = 0; s < 4; s++)
      metric_curr[s] = 1000000000;

    for (int ps = 0; ps < 4; ps++) {
      if (metric_prev[ps] >= 100000000)
        continue;

      for (int b = 0; b < 2; b++) {

        int ns = nsc_next_state[ps][b];
        int out_bit0 = nsc_output_bits[ps][b][0];
        int out_bit1 = nsc_output_bits[ps][b][1];

        int bm = branch_metric_hard_symbol(rx_bits, i, out_bit0, out_bit1);
        int cand = metric_prev[ps] + bm;

        if (cand < metric_curr[ns]) {
          metric_curr[ns] = cand;
          prev_state[i * 4 + ns] = ps;
          prev_bit[i * 4 + ns] = b;
        }
      }
    }

    for (int s = 0; s < 4; s++)
      metric_prev[s] = metric_curr[s];
  }

  /* 終端状態（STATE_A を優先） */
  int state = STATE_A;
  int best_final = metric_prev[state];
  for (int s = 0; s < 4; s++) {
    if (metric_prev[s] < best_final) {
      best_final = metric_prev[s];
      state = s;
    }
  }

  /* Backward trace */
  for (int i = steps - 1; i >= 0; i--) {
    int b = prev_bit[i * 4 + state];
    int ps = prev_state[i * 4 + state];

    if (i < K) {
      info_hat[i] = b;
    }

    state = ps;
  }

  if (code_hat) {
    nsc_encode_r05(info_hat, code_hat);
  }

  free(prev_state);
  free(prev_bit);
}
