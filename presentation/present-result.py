originPath = "Data/OUT_dbscrn_naive_lecture.csv"
newPath = "Data/OUT_dbscrn_lecture.csv"



with open(originPath,"r") as f:
    lines = f.readlines()
    points=[]
    with open(newPath,"w") as file:
        for i,line in enumerate(lines[1:]):
            words = line.split(",")
            text=""
            for i in words[:4]:
                text = text + "{0},".format(i)
            text = text[:-1] + "\n"    
            file.write(text)
            print(text)

from matplotlib import colors
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib

path = newPath

# df = pd.read_csv(r'../Data/dbscanResult.csv',names=["id","x1","x2","clasterId"])
df = pd.read_csv(path,names=["id","x1","x2","clasterId"])
# df.head()
clasters = df["clasterId"].unique()
colormap = matplotlib.cm.get_cmap('flag', clasters.size)
plot = df.plot(x="x1",y="x2",kind="scatter", c="clasterId", cmap=colormap)

# plot = df.plot(x="x1",y="x2",kind="scatter")
plt.gca().set_aspect('equal', adjustable='box')
for i, txt in enumerate(df[['id','clasterId']].values):
    plt.annotate("{0}_{1}".format(txt[0],txt[1]),(df["x1"][i],df["x2"][i]))

print(clasters)
plt.show()
print(df)