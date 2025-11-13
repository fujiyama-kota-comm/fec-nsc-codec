#ifndef NSC_ENCODER_H
#define NSC_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

extern int nsc_info_len;
extern int nsc_code_len;
extern int nsc_tail_len;

void nsc_encode_r05(const int *data, int *code);

#ifdef __cplusplus
}
#endif

#endif // NSC_ENCODER_H
