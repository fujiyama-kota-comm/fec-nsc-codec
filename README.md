# fec-nsc-codec
C implementation of **Non-Systematic Convolutional Codes (NSC)** with **Viterbi decoding**
for Forward Error Correction (FEC).
Supports **hard-decision** and **soft-decision (LLR)** decoding.

![C/C++ CI](https://github.com/fujiyama-kota-comm/fec-nsc-codec/actions/workflows/c-cpp.yml/badge.svg)

---

## 📘 Overview

This repository provides a lightweight and modular implementation of
**Non-Systematic Convolutional (NSC) Codes**, including:

- Rate-1/2 convolutional encoder
- 4-state trellis (constraint length 3)
- Viterbi decoder (hard and soft decision)
- Branchless trellis-based implementation
- BER simulation example under AWGN

Designed for:

- FEC research
- Wireless communication (5G/6G)
- Embedded systems
- Error-control coding education

---

## 📁 Project Structure

```
fec-nsc-codec
├── src/                 # Encoder/decoder implementation
├── include/             # Header files
├── examples/            # Test & BER simulation
├── Makefile             # Build script
└── .github/workflows    # CI pipeline
```

---

## 📑 Features

### ✔ NSC Encoder (Rate 1/2)
- Output bits from trellis tables
- Forced termination (tail bits)
- No conditional branching

### ✔ Viterbi Decoder
- Hard-decision decoding (Hamming metric)
- Soft-decision decoding (LLR metric)
- Full traceback implementation
- Trellis defined in `trellis.h`

### ✔ AWGN BER Simulation
`examples/test_nsc.c` evaluates BER vs Eb/N0
for both soft and hard decoding.

---

## 🛠 Build Instructions

### Requirements
- GCC or Clang
- `make`
- Linux / macOS / WSL / MinGW (Windows)

---

### Build

```sh
make
```

Generated binary:

```
test_nsc   # BER simulation program
```

Clean build files:

```sh
make clean
```

---

## 🚀 Usage Example

Run BER simulation:

```sh
./test_nsc
```

Example output:

```
EbN0_dB, BER_soft, BER_hard
0.0, 0.152300, 0.238100
1.0, 0.081200, 0.190400
2.0, 0.024800, 0.120500
3.0, 0.004900, 0.052300
...
```

---

## 📉 BER Performance

Example BER curve for rate-1/2 NSC (4-state Viterbi, AWGN, BPSK):

```
images/ber_curve.png
```

---

## 📂 Source Code Structure

### src/
| File | Description |
|------|-------------|
| `nsc_encoder.c` | NSC encoder implementation |
| `nsc_decoder.c` | Hard & soft Viterbi decoder |
| `trellis.c` | Next-state & output tables |

### include/
| File | Description |
|------|-------------|
| `nsc_encoder.h` | Encoder API |
| `nsc_decoder.h` | Decoder API |
| `trellis.h` | Trellis constants |

### examples/
| File | Description |
|------|-------------|
| `test_nsc.c` | BER simulation under AWGN |

---

## 📜 License

This project is licensed under the **MIT License**.
You may use it for research, education, and commercial applications.

---

## 🤝 Contributing

Pull requests are welcome.
For major changes, please open an issue first.

---

## ⭐ Acknowledgements

Developed as part of research in
**Forward Error Correction (FEC)** and **physical-layer communications**.

If this repository is useful, please consider starring ⭐ it on GitHub!
