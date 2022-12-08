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

a1 = subplot(2,1,1,facecolor=imageBgColour)
a1.spines['top'].set_visible(False)
a1.spines['right'].set_visible(False)
a1.spines['left'].set_color(axColour)
a1.spines['bottom'].set_color(axColour)
a1.tick_params(colors=axColour, labelcolor=txtcolour)

a2 = subplot(2,1,2,facecolor=imageBgColour,yscale="log")
a2.spines['top'].set_visible(False)
a2.spines['right'].set_visible(False)
a2.spines['left'].set_color(axColour)
a2.spines['bottom'].set_color(axColour)
a2.tick_params(colors=axColour, labelcolor=txtcolour)

def barAnim(i):
    a1.cla()
    a1.plot(sum[:i])

    a2.cla()
    a2.set_yscale("log")
    a2.bar(barX[0:start[i]],inVals[0:start[i]],color="b")
    a2.bar(barX[start[i]:end[i]], inVals[start[i]:end[i]],color="r")
    a2.bar(barX[end[i]:], inVals[end[i]:],color="b")

# barAnim(500)
# show()

# def update(i):
#     cla()
#     # axhline(invalidNumber)
#     plot(xaxis[start[i]:end[i]])
#     # yscale("log")
#     pass

ani = FuncAnimation(gcf(), barAnim,interval=25,frames=arange(0,len(start),5))
# ani.save("anim.mp4",
#          dpi=200,
#          progress_callback = lambda i, n: print(f'Saving frame {i} of {n}'))
show()