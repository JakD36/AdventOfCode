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
        return 13 * (arr[1] / arr[0]) - (2 / arr[0]) - 17
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
    while(True):
        current = func(pos)
        if current == 0:
            print(pos)
            break

        next = func(pos + dir)
        if abs(next) > abs(current):
            dir[0], dir[1] = dir[1],dir[0] # Rotate our direction
            p1 = func(pos + dir)
            p2 = func(pos - dir)

            if abs(current) < abs(p1) and abs(current) < abs(p2):
                pos += 100

            if abs(p1) > abs(p2):
                dir *= -1
        else:
            pos += dir

    return pos[0]

# def func(arr):
    #     return 37 * (arr[1] / arr[0]) - (35 / arr[0]) - 41
    #
    # print(func([204,227]))
    #
    # n1 = arange(50, 550, 20)
    # Graph(func,n1)

if __name__ == "__main__":
    Actual()
    # f = open("input.txt")
    # lines = f.readlines()
    # chars = lines[1].split(',')
    #
    # vals = []
    # for x in range(len(chars)):
    #     if chars[x].isdigit():
    #         vals.append((int(chars[x]),x))
    #
    # print(vals)
