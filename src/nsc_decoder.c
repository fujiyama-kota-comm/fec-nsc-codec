#include "nsc_decoder.h"
#include "nsc_encoder.h"
#include "trellis.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   NSC Decoder (Rate 1/2)
   - Soft decision (LLR)
   - Hard decision (0/1)
   Trellis-based Viterbi decoder
   ============================================================ */

extern int nsc_info_len; // 情報ビット長 K
extern int nsc_code_len; // 符号ビット長 N = 2*(K+2)

/* ------------------------------------------------------------
   Soft-decision branch metric (for LLR)
   出力ビットは +1 / -1 にマッピングして使う
   ------------------------------------------------------------ */
static inline double branch_metric_soft(const double *LLR, int symbol_index,
                                        int out0, int out1) {
  double v = LLR[2 * symbol_index];
  double w = LLR[2 * symbol_index + 1];

  return -(out0 * v + out1 * w);
}

/* ------------------------------------------------------------
   Hard-decision branch metric (Hamming distance)
   ------------------------------------------------------------ */
static inline int branch_metric_hard(const int *rx, int symbol_index, int out0,
                                     int out1) {
  int v = rx[2 * symbol_index];
  int w = rx[2 * symbol_index + 1];
  return (v != out0) + (w != out1);
}

/* ============================================================
   Soft-decision Viterbi Decoder
   LLR:      受信 LLR
   info_hat: 推定情報ビット（長さ K）
   code_hat: 再符号化結果（optional）
   ============================================================ */
void nsc_decode_r05_soft(const double *LLR, int *info_hat, int *code_hat) {
  int steps = nsc_code_len / 2;

  /* パスメトリック */
  double *metric[4];
  for (int s = 0; s < 4; s++) {
    metric[s] = malloc(sizeof(double) * steps);
    for (int i = 0; i < steps; i++)
      metric[s][i] = INFINITY;
  }

  /* --------------------------
     初期状態 (state=A固定)
     -------------------------- */
  for (int s = 0; s < 4; s++)
    metric[s][0] = INFINITY;

  // A→A (input 0)
  metric[STATE_A][0] = branch_metric_soft(LLR, 0, +1, +1);

  // A→C (input 1)
  metric[STATE_C][0] = branch_metric_soft(LLR, 0, -1, -1);

  /* =====================================================
     Forward recursion (trellis ベース)
     ===================================================== */
  for (int i = 1; i < steps; i++) {

    for (int s = 0; s < 4; s++) {
      double best = INFINITY;

      // どの前状態 "ps" が "s" に遷移するかを探索
      for (int ps = 0; ps < 4; ps++) {
        for (int b = 0; b < 2; b++) {
          if (nsc_next_state[ps][b] == s) {
            int out0 = nsc_output_bits[ps][b][0] ? -1 : +1;
            int out1 = nsc_output_bits[ps][b][1] ? -1 : +1;

            double m =
                metric[ps][i - 1] + branch_metric_soft(LLR, i, out0, out1);

            if (m < best)
              best = m;
          }
        }
      }
      metric[s][i] = best;
    }
  }

  /* --------------------------
     最尤終端状態
     -------------------------- */
  NSCState state = STATE_A;
  double best_final = metric[state][steps - 1];

  for (int s = 1; s < 4; s++) {
    if (metric[s][steps - 1] < best_final) {
      best_final = metric[s][steps - 1];
      state = s;
    }
  }

  /* =====================================================
     Backward recursion（情報ビット復元）
     ===================================================== */
  for (int i = steps - 1; i >= 1; i--) {

    int decided_bit = 0;
    NSCState prev_state_best = STATE_A;
    double best_metric = INFINITY;

    for (int ps = 0; ps < 4; ps++) {
      for (int b = 0; b < 2; b++) {

        if (nsc_next_state[ps][b] != state)
          continue;

        int out0 = nsc_output_bits[ps][b][0] ? -1 : +1;
        int out1 = nsc_output_bits[ps][b][1] ? -1 : +1;

        double m = metric[ps][i - 1] + branch_metric_soft(LLR, i, out0, out1);

        if (m < best_metric) {
          best_metric = m;
          prev_state_best = ps;
          decided_bit = b;
        }
      }
    }

    // i-2 以下だけ情報ビット (テールビット除外)
    if (i - 2 < nsc_info_len)
      info_hat[i - 2] = decided_bit;

    state = prev_state_best;
  }

  /* 再符号化（optional） */
  if (code_hat)
    nsc_encode_r05(info_hat, code_hat);

  for (int s = 0; s < 4; s++)
    free(metric[s]);
}

/* ============================================================
   Hard-decision Viterbi Decoder
   rx_bits: 0/1 の受信ビット列
   ============================================================ */
void nsc_decode_r05_hard(const int *rx_bits, int *info_hat, int *code_hat) {
  int steps = nsc_code_len / 2;

  /* パスメトリック */
  int *metric[4];
  for (int s = 0; s < 4; s++) {
    metric[s] = malloc(sizeof(int) * steps);
    for (int i = 0; i < steps; i++)
      metric[s][i] = 1e9;
  }

  /* 初期状態 */
  metric[STATE_A][0] = branch_metric_hard(rx_bits, 0, 0, 0);
  metric[STATE_C][0] = branch_metric_hard(rx_bits, 0, 1, 1);

  /* -----------------------------------------
     Forward recursion
     ----------------------------------------- */
  for (int i = 1; i < steps; i++) {

    for (int s = 0; s < 4; s++) {
      int best = 1e9;

      for (int ps = 0; ps < 4; ps++) {
        for (int b = 0; b < 2; b++) {

          if (nsc_next_state[ps][b] != s)
            continue;

          int out0 = nsc_output_bits[ps][b][0];
          int out1 = nsc_output_bits[ps][b][1];

          int m =
              metric[ps][i - 1] + branch_metric_hard(rx_bits, i, out0, out1);

          if (m < best)
            best = m;
        }
      }

      metric[s][i] = best;
    }
  }

  /* -----------------------------------------
     Final state
     ----------------------------------------- */
  NSCState state = STATE_A;
  int best_final = metric[state][steps - 1];

  for (int s = 1; s < 4; s++) {
    if (metric[s][steps - 1] < best_final) {
      best_final = metric[s][steps - 1];
      state = s;
    }
  }

  /* -----------------------------------------
     Backward trace
     ----------------------------------------- */
  for (int i = steps - 1; i >= 1; i--) {

    int decided_bit = 0;
    NSCState prev_state_best = STATE_A;
    int best_metric = 1e9;

    for (int ps = 0; ps < 4; ps++) {
      for (int b = 0; b < 2; b++) {

        if (nsc_next_state[ps][b] != state)
          continue;

        int out0 = nsc_output_bits[ps][b][0];
        int out1 = nsc_output_bits[ps][b][1];

        int m = metric[ps][i - 1] + branch_metric_hard(rx_bits, i, out0, out1);

        if (m < best_metric) {
          best_metric = m;
          prev_state_best = ps;
          decided_bit = b;
        }
      }
    }

    if (i - 2 < nsc_info_len)
      info_hat[i - 2] = decided_bit;

    state = prev_state_best;
  }

  if (code_hat)
    nsc_encode_r05(info_hat, code_hat);

  for (int s = 0; s < 4; s++)
    free(metric[s]);
}
