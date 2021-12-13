from matplotlib import colors
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib

paths = {"dbscan":r'../Data/dbscanResult.csv',"test":r'../Data/test.csv',"lecture":r'../Data/lecture.csv'}

path = paths["lecture"]

# df = pd.read_csv(r'../Data/dbscanResult.csv',names=["id","x1","x2","clasterId"])
df = pd.read_csv(path)
df.head()
