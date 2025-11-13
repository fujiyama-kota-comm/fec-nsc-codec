# fec-nsc-codec
C implementation of **Non-Systematic Convolutional Codes (NSC)** with **Viterbi decoding**
for Forward Error Correction (FEC).
Includes both **soft-decision** and **hard-decision** decoding, a clean **trellis table**,
and a portable **Makefile** for building on Linux/Windows/WSL.

![C/C++ CI](https://github.com/fujiyama-kota-comm/fec-nsc-codec/actions/workflows/c-cpp.yml/badge.svg)

---

## ✨ Features

- **NSC (Non-Systematic Convolutional) encoder**
  - Rate ≈ 1/2
  - 4 states (A,B,C,D) with 2-bit memory
  - Clean trellis-table-based implementation（分岐 if を使用しない）

- **Viterbi decoder**
  - Soft-decision decoding (LLR input)
  - Hard-decision decoding (Hamming distance)
  - Backward path reconstruction
  - Optional re-encoding for BER comparison

- **Modular design**
  - `src/` : encoder/decoder implementation
  - `include/` : header files
  - `examples/` : executable test scripts
  - `Makefile` : platform-independent build

- **Simulation support**
  - BPSK modulation
  - AWGN noise channel
  - BER vs Eb/N0 curve generation

- **CI-enabled**
  - GitHub Actions automatically builds and tests on every push

---

## 📦 Directory Structure

