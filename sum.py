import csv

def sum_seventh_column(csv_file_path):
    total = 0.0
    with open(csv_file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header row if your CSV has headers
        for row in reader:
            try:
                value = float(row[6].replace(',', ''))  # Remove commas and convert to float
                print(f"Processing value: {value}")  # Debug statement
                total += value
            except (IndexError, ValueError):
                # Skip rows with missing or invalid data in the 7th column
                continue
    return total

if __name__ == "__main__":
    csv_file = 'detection.csv'  # Replace with your CSV file path
    result = sum_seventh_column(csv_file)
    print(f"Sum of 7th column: {result:,.2f}")
