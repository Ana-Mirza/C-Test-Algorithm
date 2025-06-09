import pandas as pd
import matplotlib.pyplot as plt

# Citirea fișierelor CSV
magnitudes = pd.read_csv("output/magnitude.csv")
detect_peaks = pd.read_csv("output/detection.csv")
true_peaks = pd.read_csv("output/postproc.csv")

detect_peaks_on_raw = magnitudes[magnitudes['timestamp'].isin(detect_peaks['timestamp'])]
true_peaks_on_raw = magnitudes[magnitudes['timestamp'].isin(true_peaks['timestamp'])]

plt.figure(figsize=(14, 7))

# Plotare magnitudine accelerație (pre-procesare)
plt.plot(magnitudes['timestamp'], magnitudes['magnitude'], color='gray', label='Magnitudine brută')

# Adăugare puncte detectate ca vârfuri (detecție)
plt.scatter(detect_peaks_on_raw['timestamp'], detect_peaks_on_raw['magnitude'], color='blue', s=30, label='Vârfuri detectate')

# Adăugare vârfuri "adevărate" (post-procesare)
plt.scatter(true_peaks_on_raw['timestamp'], true_peaks_on_raw['magnitude'], color='red', s=60, marker='o', label='Pas confirmat')

# Afișare grafic
plt.xlabel("Timp (ms)")
plt.ylabel("Magnitudinea accelerației (mg)")
plt.title("Detectarea pașilor: Magnitudine și vârfuri")
plt.legend()
plt.grid(True)
plt.tight_layout()

OUTPUT_IMAGE = "graphs/overall_analysis.png"
plt.savefig(OUTPUT_IMAGE, dpi=300)
plt.show()
plt.close()  # Închide figura pentru a elibera memoria
print(f"Plot saved to {OUTPUT_IMAGE}")
