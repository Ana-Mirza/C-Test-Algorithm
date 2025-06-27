import pandas as pd

# Încarcă fișierul CSV (asigură-te că 'date.csv' este în același director)
df = pd.read_csv("../Dataset/validate/steps.csv")

# Calculează acuratețea pentru fiecare rând
df['accuracy'] = (1 - abs(df['stepsDetected'] - df['trueSteps']) / df['trueSteps']) * 100
print(df['accuracy'])

# Acuratețe medie, maximă și minimă pentru fiecare persoană
stats_by_person = df.groupby('person')['accuracy'].agg(['mean', 'max', 'min'])
print("Acuratețe per persoană:")
print(stats_by_person)

# Acuratețe medie, maximă și minimă pentru fiecare tip de mers
stats_by_type = df.groupby('type')['accuracy'].agg(['mean', 'max', 'min'])
print("\nAcuratețe per tip de mers:")
print(stats_by_type)
