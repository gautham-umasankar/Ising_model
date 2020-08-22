import numpy as np
import termplotlib as tpl
N = 100000
f = open("noise.csv", "w")
noise = np.random.normal(0, 1, N) 
for i in range(N):
	print(noise[i])
	f.write(str(noise[i]))
	f.write("\n")

counts, bin_edges = np.histogram(noise)

fig = tpl.figure()
fig.hist(counts, bin_edges, orientation="horizontal", force_ascii=False)
fig.show()

f.close()
