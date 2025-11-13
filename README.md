# fec-nsc-codec
C implementation of **Non-Systematic Convolutional Codes (NSC)** with **Viterbi decoding**
for Forward Error Correction (FEC).
Supports **hard-decision** and **soft-decision (LLR)** decoding.

![C/C++ CI](https://github.com/fujiyama-kota-comm/fec-nsc-codec/actions/workflows/c-cpp.yml/badge.svg)

---

## 📘 Overview

This repository provides a lightweight and modular implementation of
**Non-Systematic Convolutional (NSC) Codes** with:

- Rate-1/2 convolutional encoder
- 4-state trellis (constraint length 3)
- Viterbi decoder (hard and soft decision)
- Trellis-based implementation (no `if` branching)
- BER simulation example under AWGN

Designed for:
- FEC research
- Wireless communication (5G/6G physical layer)
- Embedded systems
- Error-control coding education

The project structure follows standard C library design:

fec-nsc-codec
├── src/ # Encoder/decoder implementation
├── include/ # Header files
├── examples/ # Test & BER simulation
├── Makefile # Build script
└── .github/workflows # CI pipeline

yaml
コードをコピーする

---

## 📑 Features

### ✔ NSC Encoder (Rate 1/2)
- Output bits generated from trellis tables
- Tail bits for forced termination
- No conditional branches (`if` removed)

### ✔ Viterbi Decoder
- Hard decision (Hamming distance metric)
- Soft decision using LLR values
- Full traceback implemented
- Trellis transition defined in `trellis.h`

### ✔ AWGN BER Simulation
Included example (`examples/test_nsc.c`) evaluates
**BER vs Eb/N0** for both soft and hard decoding.

---

## 🛠 Build Instructions

### Requirement
- GCC or Clang
- `make`
- Linux/macOS/WSL/MinGW (Windows)

### Build all targets

```sh
make
This generates:

test_nsc — BER simulation program

Clean build files
sh
コードをコピーする
make clean
🚀 Usage Example
Run BER simulation
sh
コードをコピーする
./test_nsc
Output example:

python-repl
コードをコピーする
EbN0_dB, BER_soft, BER_hard
0.0, 0.152300, 0.238100
1.0, 0.081200, 0.190400
2.0, 0.024800, 0.120500
3.0, 0.004900, 0.052300
...
📉 BER Performance
Below is an example BER curve for rate-1/2 NSC code
(4-state Viterbi, AWGN, BPSK):

(You can add your PNG later)

bash
コードをコピーする
images/ber_curve.png
📂 Source Code Structure
src/
File	Description
nsc_encoder.c	NSC encoder implementation
nsc_decoder.c	Hard & soft Viterbi decoder
trellis.c	Next-state/output tables

include/
File	Description
nsc_encoder.h	Encoder API
nsc_decoder.h	Decoder API
trellis.h	Trellis constants

examples/
File	Description
test_nsc.c	BER simulation under AWGN

📜 License
This project is licensed under the MIT License.
You are free to use it for research, education, and commercial applications.

🤝 Contributing
Pull requests are welcome.
For major changes, please open an issue first to discuss what you would like to change.

⭐ Acknowledgements
This project was developed as part of research in
Forward Error Correction (FEC) and physical-layer communications.
If this repository is useful to your research, please consider starring ⭐ it on GitHub!