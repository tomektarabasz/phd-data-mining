import numpy as np
class Point:
    attributes=np.array([1,1])

point1 = Point()
point1.attributes=np.array([1,1])
point2 = Point()
point2.attributes =np.array([0,1])

def tanimoto_dist(point1,point2):
    attr1 = point1.attributes
    attr2 = point2.attributes
    inner_product = attr1@attr2 #this is inner product 
    squareLength1 = attr1@attr1 #squared length of vector calculated as inner product of itself
    squareLength2 = attr2@attr2 #squared length of vector calculated as inner product of itself
    print(inner_product)
    print(squareLength1)
    print(squareLength2)
    tanimotoSimilarity = inner_product/(squareLength1+squareLength2-inner_product)
    tanimotoDist = 1 - tanimotoSimilarity
    print(tanimotoDist)
    return tanimotoDist
    
tanimoto_dist(point1,point2)
