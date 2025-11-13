#include "trellis.h"

/* ============================================================
   NSC Trellis for Rate-1/2 Non-Systematic Convolutional Code
   状態 A,B,C,D と 入力 0/1 に対する
   - 出力ビット (v,w)
   - 次状態
   を完全再現したテーブル
   ============================================================ */

/* 出力 (v,w) */
const int nsc_output_bits[4][2][2] = {
    /* STATE_A (00) */
    {
        {0, 0}, // input 0 → 00 → A
        {1, 1}  // input 1 → 11 → C
    },
    /* STATE_B (01) */
    {
        {1, 1}, // input 0 → 11 → A
        {0, 0}  // input 1 → 00 → C
    },
    /* STATE_C (10) */
    {
        {0, 1}, // input 0 → 01 → B
        {1, 0}  // input 1 → 10 → D
    },
    /* STATE_D (11) */
    {
        {1, 0}, // input 0 → 10 → B
        {0, 1}  // input 1 → 01 → D
    }};

/* 次状態 */
const int nsc_next_state[4][2] = {
    /* STATE_A */ {STATE_A, STATE_C},
    /* STATE_B */ {STATE_A, STATE_C},
    /* STATE_C */ {STATE_B, STATE_D},
    /* STATE_D */ {STATE_B, STATE_D}};
