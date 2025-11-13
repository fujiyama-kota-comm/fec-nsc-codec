#ifndef NSC_DECODER_H
#define NSC_DECODER_H

#ifdef __cplusplus
extern "C" {
#endif

void nsc_decode_r05_soft(const double *LLR, int *info_hat, int *code_hat);

void nsc_decode_r05_hard(const int *rx_bits, int *info_hat, int *code_hat);

#ifdef __cplusplus
}
#endif

#endif // NSC_DECODER_H
