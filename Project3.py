
import pandas as pd
import xlrd
api_key = "AIzaSyB1WM07u8JJeYzp9WA26MJEkT-wpwXeQxY"
filePath = r"C:\Users\murar\Documents\UF School\Fall 2021\Data Structures\Alternative_Excel.xlsx"
df = pd.read_excel(filePath)

print(df.columns)

df.drop(['Intersection_Directions'], axis=1)

print(df.columns)

df1 = pd.DataFrame()

df1.join(df['X'])
df1.join(df['Y'])
df1.join(df['Street_Address'])
df1.join(df['City'])
df1.join(df['State'])
df1.join(df['ZIP'])

endpoint = r"C:\Users\murar\Documents\UF School\Fall 2021\Data Structures\Alternative_Excel.csv"
df1.to_csv(endpoint)
df.to_csv(r"C:\Users\murar\Documents\UF School\Fall 2021\Data Structures\FULL_Alternative_Excel.csv")


#need to drop cols in df