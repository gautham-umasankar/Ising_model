from numpy import *
from fxpmath import Fxp

sig = 0.3
mu = 0

f = open("./matmul.h","r")
N = 1
n_int = 7
n_frac = 9
for i in f.readlines():
    line = i.split()
    if(len(line) > 2):
        if(line[1] == "N"):
            N = int(line[2])
            print("N = ", N)
        elif(line[0] == "typedef" and line[1][0] == "a"):
            nt = line[-1].strip(";").split("_")
            n_int = int(nt[1])
            n_frac = int(nt[2])
            print("Int bits = ",n_int)
            print("Frac bits = ", n_frac)
            break
f.close()

mat = random.normal(mu, sig, (N, N))
in_vec = random.normal(mu, sig, (N, 1))
# mat_file = open("../testcases/mat_file_float.txt","w")
# for i in range(N):
#     for j in range(N):
#         mat_file.write(str(mat[i][j]))
#         mat_file.write(" ")
#     mat_file.write("\n")
# mat_file.close()
mat = Fxp(mat, signed = True, n_word = n_int+n_frac, n_frac = n_frac)
mat_file = open("../testcases/mat_file_fp.txt","w")
for i in range(N):
    for j in range(N):
        mat_file.write(str(mat[i][j]))
        mat_file.write(" ")
    mat_file.write("\n")
mat_file.close()

# in_vec_file = open("../testcases/in_vec_file_float.txt","w")
# for i in range(N):
#     in_vec_file.write(str(in_vec[i][0]))
#     in_vec_file.write("\n")
# in_vec_file.close()
in_vec = Fxp(in_vec, signed = True, n_word = n_int+n_frac, n_frac = n_frac)
in_vec_file = open("../testcases/in_vec_file_fp.txt","w")
for i in range(N):
    in_vec_file.write(str(in_vec[i][0]))
    in_vec_file.write("\n")
in_vec_file.close()

out_vec = dot(mat(), in_vec())
# out_vec_file = open("../testcases/out_vec_file_float.txt","w")
# for i in range(N):
#     out_vec_file.write(str(out_vec[i][0]))
#     out_vec_file.write("\n")
# out_vec_file.close()

out_vec = Fxp(out_vec, signed = True, n_word = 2*(n_int+n_frac), n_frac = 2*n_frac)

out_vec_file = open("../testcases/out_vec_file_fp.txt","w")
for i in range(N):
    out_vec_file.write(str(out_vec[i][0]))
    out_vec_file.write("\n")
out_vec_file.close()

# print(mat)
# print(in_vec)
# print(out_vec)
