import termplotlib as tpl

f = open("./out_data/x_n.csv","r")
y = f.readlines()
x = list(range(len(y)))
fig = tpl.figure()
fig.plot(x, y, width = 100, height = 50)
fig.show()

