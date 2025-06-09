import matplotlib.pyplot as plt
import csv

csv_files = {
    "output/magnitude.csv": "graphs/magnitude.png",
    "output/interpolated.csv": "graphs/interpolated.png",
    "output/filtered.csv": "graphs/filtered.png",
    "output/scoring.csv": "graphs/scoring.png",
    "output/detection.csv": "graphs/detection.png",
    "output/postproc.csv": "graphs/postproc.png",
}

# === CONFIGURATION ===

timestamps = []
magnitudes = []

for CSV_FILE, OUTPUT_IMAGE in csv_files.items():
    timestamps.clear()
    magnitudes.clear()

    plt.ioff()  # Disable interactive mode to avoid showing plots immediately

    # Read CSV file
    with open(CSV_FILE, newline='') as csvfile:
        reader = csv.reader(csvfile)
        next(reader)  # Skip header if present, comment this if there's no header
        for row in reader:
            timestamps.append(float(row[0]))
            magnitudes.append(float(row[1]))

    # Plotting
    plt.figure(figsize=(12, 6))
    plt.plot(timestamps, magnitudes, marker='o', linestyle='-', color='blue', label='Magnitudine')
    plt.xlabel('Timp (ms)')
    plt.ylabel('Magnitudine accelerație')
    plt.title('Magnitudinea accelerației în funcție de timp')
    plt.legend()
    plt.grid(True)

    # Show or save
    plt.savefig(OUTPUT_IMAGE, dpi=300)
    plt.show()
    plt.close()  # Close the figure to free memory
    print(f"Plot saved to {OUTPUT_IMAGE}")