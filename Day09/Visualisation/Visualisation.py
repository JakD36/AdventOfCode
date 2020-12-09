from PlotUtils import *
from matplotlib.animation import FuncAnimation

invalidNumber = 393911906

f = open("progress.txt",'r')
count = int(f.readline())
xaxis = range(0,count)

start = []
end = []
sum = []
for x in f.readlines():
    a,b,c = x.split(' ')
    start.append(int(a))
    end.append(int(b))
    sum.append(int(c))

inVals = []
f = open("../input.txt",'r')
for x in f.readlines():
    inVals.append(int(x))

barX = range(len(inVals))

setup2d()

def barAnim(i):
    cla()
    bar(barX[0:start[i]],inVals[0:start[i]],color="b")
    bar(barX[start[i]:end[i]], inVals[start[i]:end[i]],color="r")
    bar(barX[end[i]:], inVals[end[i]:],color="b")
    yscale("log")

# def update(i):
#     cla()
#     # axhline(invalidNumber)
#     plot(xaxis[start[i]:end[i]])
#     # yscale("log")
#     pass

ani = FuncAnimation(gcf(), barAnim,interval=25,frames=arange(0,len(start),10))
ani.save("anim.mp4",
         dpi=150,
         progress_callback = lambda i, n: print(f'Saving frame {i} of {n}'))
# show()