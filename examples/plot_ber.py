import pandas as pd
import matplotlib.pyplot as plt
import os

# ▼ 出力フォルダ作成（自動）
os.makedirs("images", exist_ok=True)

# 論文向けフォント設定
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["mathtext.fontset"] = "stix"
plt.rcParams["font.size"] = 14

# CSV読み込み
df = pd.read_csv("results/ber_result.csv")  # ← results から読み込む

EbN0 = df["EbN0_dB"]
ber_soft = df["BER_soft"]
ber_hard = df["BER_hard"]
ber_bpsk = df["BER_bpsk"]

# グラフ作成
plt.figure(figsize=(7.5, 6))

# Soft Viterbi
plt.semilogy(
    EbN0,
    ber_soft,
    marker="o",
    markersize=8,
    markerfacecolor="none",
    markeredgewidth=1.8,
    linewidth=2.5,
    label="Soft-decision Viterbi",
)

# Hard Viterbi
plt.semilogy(
    EbN0,
    ber_hard,
    marker="s",
    markersize=8,
    markerfacecolor="none",
    markeredgewidth=1.8,
    linewidth=2.5,
    label="Hard-decision Viterbi",
)

# Uncoded BPSK
plt.semilogy(
    EbN0,
    ber_bpsk,
    marker="^",
    markersize=8,
    markerfacecolor="none",
    markeredgewidth=1.8,
    linewidth=2.5,
    label="Uncoded BPSK (theory)",
)

# Y軸範囲
plt.ylim(1e-5, 1)

# 軸ラベル
plt.xlabel("Eb/N0 [dB]", fontsize=18)
plt.ylabel("Bit Error Rate (BER)", fontsize=18)

plt.grid(True, which="both", linestyle="--", linewidth=0.6, alpha=0.6)
plt.legend(fontsize=14, loc="upper right", frameon=True, edgecolor="black")

plt.tight_layout()

# ▼▼ ここで images/ フォルダに保存 ▼▼
plt.savefig("images/ber_curve.pdf", dpi=300, bbox_inches="tight")
plt.savefig("images/ber_curve.png", dpi=300, bbox_inches="tight")

plt.show()
