from numpy import *
f = open("./matmul.h","r")
N = 1
for i in f.readlines():
    line = i.split()
    if(len(line) > 2):
        if(line[1] == "N"):
            N = int(line[2])
f.close()

mat = random.randint(0, 64, (N, N))
mat_file = open("mat_file.txt","w")
for i in range(N):
    for j in range(N):
        mat_file.write(str(mat[i][j]))
        mat_file.write(" ")
    mat_file.write("\n")
mat_file.close()

in_vec = random.randint(0, 32, (N, 1))
in_vec_file = open("in_vec_file.txt","w")
for i in range(N):
    in_vec_file.write(str(in_vec[i][0]))
    in_vec_file.write("\n")
in_vec_file.close()

out_vec = mat.dot(in_vec)
out_vec_file = open("out_vec_file.txt","w")
for i in range(N):
    out_vec_file.write(str(out_vec[i][0]))
    out_vec_file.write("\n")
out_vec_file.close()

print(mat)
print(in_vec)
print(out_vec)
