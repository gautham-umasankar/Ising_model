import matplotlib.pyplot as plt
data=''
with open("different_temp.txt") as file:
    data=file.read()

cut=[]
tim=[]
t=[]
for line in data.split("\n")[:-1]:
    line=line.strip()
    a,b,c=line.split();
    cut.append(-int(b))
    tim.append(int(c))
    t.append(int(a)
