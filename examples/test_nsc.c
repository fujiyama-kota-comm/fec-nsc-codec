#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "nsc_decoder.h"
#include "nsc_encoder.h"

#define PI 3.14159265358979323846 // 円周率

/* 乱数 */
double randn() {
  double u1 = (rand() + 1.0) / (RAND_MAX + 2.0);
  double u2 = (rand() + 1.0) / (RAND_MAX + 2.0);
  return sqrt(-2.0 * log(u1)) * cos(2 * PI * u2);
}

int main() {

  /* パラメータ設定 */
  nsc_info_len = 100;           // K
  nsc_code_len = 2 * (100 + 2); // 204

  int K = nsc_info_len;
  int N = nsc_code_len;

  int *data = malloc(sizeof(int) * K);
  int *code = malloc(sizeof(int) * N);

  double *LLR = malloc(sizeof(double) * N);
  int *rx_bits = malloc(sizeof(int) * N);

  int *info_soft = malloc(sizeof(int) * K);
  int *info_hard = malloc(sizeof(int) * K);
  int *code_hat = malloc(sizeof(int) * N);

  srand(time(NULL));

  printf("EbN0_dB, BER_soft, BER_hard\n");

  /* Eb/N0 を 0~6 dB で sweep */
  for (double EbN0_dB = 0; EbN0_dB <= 6; EbN0_dB += 1.0) {

    double EbN0 = pow(10.0, EbN0_dB / 10.0);
    double R = 0.5;
    double sigma2 = 1.0 / (2.0 * R * EbN0);
    double sigma = sqrt(sigma2);

    int total_bits = 0;
    int error_soft = 0;
    int error_hard = 0;

    /* Monte Carlo 回す（1万ビット程度）*/
    int trials = 200; // もっと増やしてもOK（例：1000）
    for (int t = 0; t < trials; t++) {

      /* ランダム情報ビット生成 */
      for (int i = 0; i < K; i++)
        data[i] = rand() & 1;

      /* NSC符号化 */
      nsc_encode_r05(data, code);

      /* BPSK変調 + AWGN */
      for (int i = 0; i < N; i++) {
        double s = (code[i] == 0) ? +1.0 : -1.0; // BPSK
        double y = s + sigma * randn();

        /* ソフト LLR */
        LLR[i] = 2.0 * y / sigma2;

        /* ハード判定用 */
        rx_bits[i] = (y >= 0) ? 0 : 1;
      }

      /* 復号 */
      nsc_decode_r05_soft(LLR, info_soft, code_hat);
      nsc_decode_r05_hard(rx_bits, info_hard, code_hat);

      /* BER count */
      for (int i = 0; i < K; i++) {
        if (info_soft[i] != data[i])
          error_soft++;
        if (info_hard[i] != data[i])
          error_hard++;
      }

      total_bits += K;
    }

    printf("%.1f, %.6f, %.6f\n", EbN0_dB, (double)error_soft / total_bits,
           (double)error_hard / total_bits);
  }

  free(data);
  free(code);
  free(LLR);
  free(rx_bits);
  free(info_soft);
  free(info_hard);
  free(code_hat);

  return 0;
}
