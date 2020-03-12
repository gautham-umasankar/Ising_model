from numpy import *
data=''
with open("input.txt") as file:
    data=file.read()
l1=data.split("\n")[0]
n,m=l1.split()
n=int(n);m=int(m)

l=zeros(shape=(n,n))

for line in data.split("\n")[1:-1]:
    print(line)
    line=line.strip()
    x,y,w=line.split()
    x=int(x)-1
    y=int(y)-1
    w=int(w)
    l[x,y]=w
    l[y,x]=w