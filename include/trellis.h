#ifndef TRELLIS_H
#define TRELLIS_H

/* 4状態定義（エンコーダに合わせる） */
typedef enum {
  STATE_A = 0, // 00
  STATE_B = 1, // 01
  STATE_C = 2, // 10
  STATE_D = 3  // 11
} NSCState;

/* 出力ビット表（v,w）: nsc_output_bits[state][input][0 or 1] */
extern const int nsc_output_bits[4][2][2];

/* 次状態表: nsc_next_state[state][input] */
extern const int nsc_next_state[4][2];

#endif
