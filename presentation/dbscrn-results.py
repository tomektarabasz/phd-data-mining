from matplotlib import colors
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib

paths = {"dbscrn-optim":r'./Data/dbscrnResults.csv'}

path = paths["dbscrn-optim"]

# df = pd.read_csv(r'../Data/dbscanResult.csv',names=["id","x1","x2","clasterId"])
df = pd.read_csv(path,names=["id","x1","x2","clasterId"])
# df.head()
clasters = df["clasterId"].unique()
# colors = ["red", "yellow", "purple","green", "black", "blue", "pink", "orange", "silver", "brown"]
colormap = matplotlib.cm.get_cmap("autumn", clasters.shape[0])
plot = df.plot(x="x1",y="x2",kind="scatter", c="clasterId", cmap=colormap)
# plot = df.plot(x="x1",y="x2",kind="scatter")
plt.gca().set_aspect('equal', adjustable='box')
for i, txt in enumerate(df[['id','clasterId']].values):
    plt.annotate("{0}_{1}".format(txt[0],txt[1]),(df["x1"][i],df["x2"][i]))

print(clasters)
plt.show()
print(df)