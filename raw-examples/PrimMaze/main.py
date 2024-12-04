import prim

import matplotlib.pyplot as plt

from matplotlib.axes import Axes
from matplotlib.ticker import MultipleLocator

def fill_grid_with_color(ax: Axes, x_left_bottom, y_right_bottom, color = 'black'):
    x_line_points = [x_left_bottom, x_left_bottom + 1]
    y1 = y_right_bottom
    y2 = y_right_bottom + 1
    ax.fill_between(x_line_points, y1, y2, facecolor=color)

def main():
    ax = plt.subplot()

    X = 31
    Y = 31

    plt.xlim(0, X)
    plt.ylim(0, Y)

    ax.xaxis.set_major_locator(MultipleLocator(1))
    ax.yaxis.set_major_locator(MultipleLocator(1))

    ax.xaxis.grid(True, which='major')
    ax.yaxis.grid(True, which='major')

    ax.invert_yaxis()
    ax.xaxis.tick_top()

    pm = prim.PrimMaze(Y, X)
    pm.generate()

    for row in range(pm.row_max):
        for col in range(pm.col_max):
            if pm.grids[row, col] == pm.WALL:
                fill_grid_with_color(ax, col, row)

    plt.show()

if __name__ == '__main__':
    main()
