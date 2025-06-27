import pandas as pd
import matplotlib.pyplot as plt

# Citirea fișierelor CSV
magnitudes = pd.read_csv("magnitude.csv", header=None)
filtered_magnitudes = pd.read_csv("filtered.csv", header=None)
scored_magnitudes = pd.read_csv("scoring.csv", header=None)
detect_peaks = pd.read_csv("detection.csv", header=None)
true_peaks = pd.read_csv("postproc.csv", header=None)

detect_peaks_on_raw = magnitudes[magnitudes[0].isin(detect_peaks[0])]
true_peaks_on_raw = magnitudes[magnitudes[0].isin(true_peaks[0])]

plt.figure(figsize=(14, 7))

# Plotare magnitudine accelerație (pre-procesare)
plt.plot(magnitudes[0], magnitudes[1], color='gray', label='Magnitudine brută')
plt.plot(filtered_magnitudes[0], filtered_magnitudes[1], color='green', label='Magnitudine filtrată')
plt.plot(scored_magnitudes[0], scored_magnitudes[1], color='darkgreen', label='Magnitudine cu scor')
# plt.plot(detect_peaks[0], detect_peaks[1], color='orange', label='Vârfuri detectate')
# plt.plot(true_peaks[0], true_peaks[1], color='purple', label='Vârfuri "adevărate" (post-procesare)')

# Adăugare puncte detectate ca vârfuri (detecție)
plt.scatter(detect_peaks[0], detect_peaks[1], color='orange', s=30, label='Vârfuri detectate')

# Adăugare vârfuri "adevărate" (post-procesare)
plt.scatter(true_peaks[0], true_peaks[1], color='purple', s=60, marker='o', label='Pas confirmat')

# Afișare grafic
plt.xlabel("Timp (ms)", fontsize=18)
plt.ylabel("Magnitudinea accelerației (m/s²)", fontsize=15)
plt.title("")
plt.legend()
plt.grid(True)
plt.tight_layout()

OUTPUT_IMAGE = "graphs/overall_analysis.png"
plt.savefig(OUTPUT_IMAGE, dpi=300)
plt.show()
plt.close()  # Închide figura pentru a elibera memoria
print(f"Plot saved to {OUTPUT_IMAGE}")


# Plot another graph with different colors
plt.figure(figsize=(14, 7))
# Plotare magnitudine accelerație (pre-procesare)
plt.plot(magnitudes[0], magnitudes[1], color='gray', label='Magnitudine brută')
plt.scatter(detect_peaks_on_raw[0], detect_peaks_on_raw[1], color='orange', s=30, label='Vârfuri detectate pe date brute')
plt.scatter(true_peaks_on_raw[0], true_peaks_on_raw[1], color='purple', s=60, marker='o', label='Pas confirmat pe date brute')
# Afișare grafic
plt.xlabel("Timp (ms)")
plt.ylabel("Magnitudinea accelerației (m/s²)")
plt.title("")
plt.legend()
plt.grid(True)
plt.tight_layout()
OUTPUT_IMAGE = "graphs/overall_analysis_raw.png"
plt.savefig(OUTPUT_IMAGE, dpi=300)
plt.show()
plt.close()
print(f"Plot saved to {OUTPUT_IMAGE}")


