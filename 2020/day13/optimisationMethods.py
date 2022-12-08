from Day13.PlotUtils import *

def Graph(func, x):
    setup3d()

    n = [n1, n2] = meshgrid(x, x)

    o = func(n)

    gca().set_zlim(-50,50)
    gca().plot_surface(n1, n2, o,
                       rstride=1,
                       cstride=1,
                       )

    plot(0,0,0)
    show()


def Example():
    """
    17x == 13y - 2 == our answer
    After rearranging we get 13(y/x) - 2/x - 17 == 0
    If we solve for this we can multiply the x value we find by 17 we get our answer
    """
    def func(arr):
        return 13 * arr[1] - 2 - 17 * arr[0]
    initPos = array([200, 200])
    x = Solve(initPos, func)
    print(17 * x)

def Actual():
    """
    41x == 37y - 35 == our answer
    After rearranging we get 37y - 35 - 41x == 0
    If we solve for this we can multiply the x value we find by 41 we get our answer
    """
    def func(arr):
        return 37 * arr[1] - 35 - 41 * arr[0]

    initPos = array([3145908451612, 3486006662612])
    # initPos = array([3_225_806_451_612, 3_225_806_451_612])
    x = Solve(initPos, func)
    # print(41 * x)

def Solve(initPos, func):
    step = 1
    pos = initPos
    dir = array([step,0])
    current = func(pos)
    while(True):
        pass
        # Find Direction
        p1 = abs(func(pos + array([-1, 1])))
        p2 = abs(func(pos + array([0, 1])))
        p3 = abs(func(pos + array([1, 1])))

        p4 = abs(func(pos + array([-1, 0])))
        p5 = abs(func(pos + array([1, 0])))

        p6 = abs(func(pos + array([-1, -1])))
        p7 = abs(func(pos + array([0, -1])))
        p8 = abs(func(pos + array([1, -1])))


    return pos[0]

# def func(arr):
    #     return 37 * (arr[1] / arr[0]) - (35 / arr[0]) - 41
    #
    # print(func([204,227]))
    #
    # n1 = arange(50, 550, 20)
    # Graph(func,n1)

if __name__ == "__main__":
    # x = arange(0,4000,1)
    # y1 = (1/17 * x) % 1
    # y2 = (1/13 * (x + 2) ) % 1
    # y3 = (1/19 * (x + 3)) % 1
    # setup2d()
    # plot(x, y1)
    # plot(x, y2)
    # plot(x, y3)
    # xlim(100, 110)
    # ylim(0, 1)
    # show()
    # These all equal 0 at the same time at 3417

    # print(1 / 17 * 3417)
    # print(1 / 13 * (3417+2))
    # print(1 / 19 * (3417+3))

    # x = arange(0,100,1)
    # y1 = 6 + 13 * x
    # y2 = 11 + 19 * x
    # setup2d()
    # plot(x, y1)
    # plot(x, y2)
    # show()

    def meet(a,b):
        def func(n):
            return (a[0] * n - a[1] + b[1]) / b[0]
        init = -1
        for x in range(1,b[0]) :
            result = func(x)
            if result % 1 == 0:
                init = -x
                break
        repeats = b[0]
        return (repeats, init)

    f = open("input.txt")
    lines = f.readlines()
    chars = lines[1].split(',')
    vals = []
    for x in range(len(chars)):
        if chars[x].strip().isdigit():
            vals.append((int(chars[x]),x))

    meets = []
    for x in range(1,len(vals)):
        meets.append(meet(vals[0],vals[x]))

    next = []
    congragate = meets[0]
    for x in range(1,len(meets)):
        tmp = meet(congragate,meets[x])
        congragate = (congragate[0] * tmp[0], congragate[0] * tmp[1] + congragate[1])

    print(congragate[1] * vals[0][0])

