#include "nsc_encoder.h"
#include "trellis.h"
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   Non-Systematic Convolutional (NSC) Encoder   Rate ≈ 1/2
   Trellis-based implementation (if文なし)
   ============================================================ */

// グローバル変数の実体
int nsc_info_len;     // 情報ビット長 K
int nsc_code_len;     // 符号ビット長 N (= 2*(K+tail))
int nsc_tail_len = 2; // テールビット長

/* ------------------------------------------------------------
   NSC Encoder (Rate 1/2)
   data: 入力ビット列（長さ K）
   code: 出力系列（長さ N = 2*(K+2)）
   ------------------------------------------------------------ */
void nsc_encode_r05(const int *data, int *code) {
  int total_bits = nsc_info_len + nsc_tail_len;

  /* 入力＋テールビット */
  int *buffer = malloc(sizeof(int) * total_bits);
  if (!buffer) {
    fprintf(stderr, "Memory allocation failed\n");
    return;
  }

  for (int i = 0; i < nsc_info_len; i++)
    buffer[i] = data[i];

  buffer[nsc_info_len] = 0;     // テール1
  buffer[nsc_info_len + 1] = 0; // テール2

  /* 初期状態は A(00) */
  int state = STATE_A;

  /* ============================================================
     Trellis に従って遷移（if 文なし）
     ============================================================ */
  for (int i = 0; i < total_bits; i++) {

    int b = buffer[i]; // 入力ビット（0/1）

    /* 出力2ビット */
    code[2 * i] = nsc_output_bits[state][b][0];
    code[2 * i + 1] = nsc_output_bits[state][b][1];

    /* 次状態に遷移 */
    state = nsc_next_state[state][b];
  }

  free(buffer);
}
