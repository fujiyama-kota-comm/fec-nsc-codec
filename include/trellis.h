#ifndef NSC_TRELLIS_H
#define NSC_TRELLIS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================
   NSC (Non-Systematic Convolutional Code) Trellis
   ---------------------------------------------------------------
   状態数: 4 (00=A, 01=B, 10=C, 11=D)
   入力ビット: 0 / 1
   出力ビット: 各入力に対し2ビット (rate 1/2)
   ================================================================ */

// 状態番号
typedef enum {
  STATE_A = 0, // 00
  STATE_B = 1, // 01
  STATE_C = 2, // 10
  STATE_D = 3  // 11
} NSCState;

// 次状態テーブル: next_state[current_state][input_bit]
extern const int nsc_next_state[4][2];

// 出力ビット: nsc_output_bits[current_state][input_bit][0 or 1]
extern const int nsc_output_bits[4][2][2];

#ifdef __cplusplus
}
#endif

#endif // NSC_TRELLIS_H
