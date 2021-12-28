with open("Data/dim512.txt","r") as f:
    lines = f.readlines()
    points=[]
    with open("Data/dim512.csv","w") as file:
        for i,line in enumerate(lines):
            text ="p{0}".format(i)+line.replace("   ",",");
            file.write(text)
            print(text)