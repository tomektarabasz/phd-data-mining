with open("Data/letter.txt","r") as f:
    lines = f.readlines()
    points=[]
    with open("Data/letter.csv","w") as file:
        for i,line in enumerate(lines):
            text ="p{0},".format(i)+line[0:-2]+"\n"
            file.write(text)
            print(text)