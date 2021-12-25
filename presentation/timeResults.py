from matplotlib import colors
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib

paths = {"time":r'./Data/time.csv'}

path = paths["time"]

# df = pd.read_csv(r'../Data/dbscanResult.csv',names=["id","x1","x2","clasterId"])
df = pd.read_csv(path)
print(df)
