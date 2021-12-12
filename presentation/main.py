from matplotlib import colors
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib

paths = {"dbscan":r'../Data/dbscanResult.csv',"test":r'../Data/test.csv',"lecture":r'../Data/lecture.csv'}

path = paths["lecture"]

# df = pd.read_csv(r'../Data/dbscanResult.csv',names=["id","x1","x2","clasterId"])
df = pd.read_csv(path,names=["id","x1","x2","clasterId"])
# df.head()
clasters = df["clasterId"].unique()
# colors = ["red", "yellow", "purple","green", "black", "blue", "pink", "orange", "silver", "brown"]
colormap = matplotlib.cm.get_cmap("autumn", clasters.shape[0])
# plot = df.plot(x="x1",y="x2",kind="scatter", c="clasterId", cmap=colormap)
plot = df.plot(x="x1",y="x2",kind="scatter")

for i, txt in enumerate(df["id"]):
    plt.annotate(txt,(df["x1"][i],df["x2"][i]))

print(clasters)
plt.show()
print(df)