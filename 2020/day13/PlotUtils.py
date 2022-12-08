from mpl_toolkits.mplot3d import Axes3D
from matplotlib.pyplot import *
from numpy import *

#Colours
txtcolour = [0.8,0.8,0.8]
imageBgColour = [50/255,50/255,52/255]
axColour = [0.8,0.8,0.8]

def setup2d():
    figure(0,
           # dpi=300,
           # figsize=[6,4],
           facecolor=imageBgColour,
           edgecolor=imageBgColour,
           frameon=True,
           )

    a = gca(
        # xlim=[0,5],
        xscale = "linear",
        # ylim = [],
        yscale = "linear",
        # yticks = [10,20,30,40,50],
        facecolor=imageBgColour,
        # edgecolor="w",
    )

    # Have to use the OO api to accomplish what I want
    a.spines['top'].set_visible(False)
    a.spines['right'].set_visible(False)
    a.spines['left'].set_color(axColour)
    a.spines['bottom'].set_color(axColour)

    # box(on=False) // Completely hides the box around the plot, but leaves the ticks

    # minorticks_on()
    # grid(b=True,color="blue",which="both")

    tick_params(colors=axColour, labelcolor=txtcolour)


def example2d():
    def RatingToPercent01(rating, enemyLevel):
        return rating / (5 * enemyLevel + rating)

    x = arange(0, 100, 1) # level
    y = RatingToPercent01(5,x)

    setup2d()

    plot(x, y, label="Rating 5",
         linestyle="-",
         linewidth = 1,
         color="green",
         marker="",
         markeredgecolor = "red",
         markeredgewidth = 0.2,
         markerfacecolor = "green",
         markersize = 3,
         )

    xlabel("X axis (units)", color=txtcolour)
    ylabel("Y axis (units)", color=txtcolour)
    title("Sample Graph", color=txtcolour)

    legend(
        loc = "best",
        frameon=False,
        facecolor="k",
        edgecolor="w",
        labelcolor="linecolor"
    )

    # savefig("figure.png",
    #         facecolor= imageBgColour,
    #         transparent=True
    #         )
    show()

def setup3d():
    figure(0,
           dpi=300,
           figsize=[3, 2],
           facecolor=imageBgColour,
           edgecolor=imageBgColour,
           frameon=True,
           )

    a = gca(projection='3d',
            # xlim=[0,5],
            xscale="linear",
            # ylim = [],
            yscale="linear",
            # yticks = [10,20,30,40,50],
            facecolor=imageBgColour,
            # edgecolor="w",
            )

    # Have to use the OO api to accomplish what I want
    # a.spines['top'].set_visible(False)
    # a.spines['right'].set_visible(False)
    # a.spines['left'].set_color(axColour)
    # a.spines['bottom'].set_color(axColour)

    # box(on=False) // Completely hides the box around the plot, but leaves the ticks

    # minorticks_on()
    # grid(b=True,color="blue",which="both")

    tick_params(colors=axColour, labelcolor=txtcolour)

def example3d():
    def RatingToPercent01(rating, enemyLevel):
        return rating / (1 * enemyLevel + rating)

    setup3d()

    rating = arange(1, 100, 1)
    enemyLevel = arange(1, 100, 1)
    rating, enemyLevel = meshgrid(rating, enemyLevel)

    percent = RatingToPercent01(rating, enemyLevel)

    gca().plot_surface(rating, enemyLevel, percent,
                       rstride=2,
                       cstride=2,
                       )

    xlabel("Rating", color=txtcolour)
    ylabel("Enemy Level", color=txtcolour)
    title("rating / (5 * enemyLevel + rating)", color=txtcolour)
    # ax.set_zlabel("Z axis")

    show()