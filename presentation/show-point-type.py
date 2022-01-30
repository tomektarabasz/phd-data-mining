originPath = "Data/OUT_dbscan_.csv"
newPath = "Data/OUT_dbscan_-cuted.csv"



with open(originPath,"r") as f:
    lines = f.readlines()
    points=[]
    with open(newPath,"w") as file:
        for i,line in enumerate(lines[1:]):
            words = line.split(",")
            text=""
            for i in words[:5]:
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
df = pd.read_csv(path,names=["id","x1","x2","clasterId","pointType"])
# df.head()
clasters = df["pointType"].unique()
# colors = ["red", "yellow", "purple","green", "black", "blue", "pink", "orange", "silver", "brown"]
colormap = matplotlib.cm.get_cmap("autumn", clasters.shape[0])
plot = df.plot(x="x1",y="x2",kind="scatter", c="pointType", cmap=colormap)
# plot = df.plot(x="x1",y="x2",kind="scatter")
plt.gca().set_aspect('equal', adjustable='box')
# for i, txt in enumerate(df[['id','pointType']].values):
#     plt.annotate("{0}_{1}".format(txt[0],txt[1]),(df["x1"][i],df["x2"][i]))

print(clasters)
plt.show()
print(df)