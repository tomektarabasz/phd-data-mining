with open("Data/cluto-t7-10k.txt","r") as f:
    lines = f.readlines()
    points=[]
    with open("Data/cluto-t7-10k.csv","w") as file:
        for i,line in enumerate(lines):
            arg = line.split(",");
            text ="p{0},".format(i)+arg[0]+","+arg[1]+"\n"
            file.write(text)
            print(text)