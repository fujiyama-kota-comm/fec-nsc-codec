#include "trellis.h"

/* =====================================================================
   NSC Trellis Table (R = 1/2, 2-bit shift register, 4 states)
   =====================================================================
   State A (00)
       input 0 → A (00), output 00
       input 1 → C (10), output 11

   State B (01)
       input 0 → A (00), output 11
       input 1 → C (10), output 00

   State C (10)
       input 0 → B (01), output 01
       input 1 → D (11), output 10

   State D (11)
       input 0 → B (01), output 10
       input 1 → D (11), output 01
   ===================================================================== */

// 次状態テーブル
const int nsc_next_state[4][2] = {
    // input 0 , input 1
    {STATE_A, STATE_C}, // STATE_A
    {STATE_A, STATE_C}, // STATE_B
    {STATE_B, STATE_D}, // STATE_C
    {STATE_B, STATE_D}  // STATE_D
};

// 出力ビットテーブル（2bit）
const int nsc_output_bits[4][2][2] = {
    // STATE_A
    {{0, 0}, {1, 1}}, // input 0 → 00, input 1 → 11

    // STATE_B
    {{1, 1}, {0, 0}}, // input 0 → 11, input 1 → 00

    // STATE_C
    {{0, 1}, {1, 0}}, // input 0 → 01, input 1 → 10

    // STATE_D
    {{1, 0}, {0, 1}} // input 0 → 10, input 1 → 01
};
