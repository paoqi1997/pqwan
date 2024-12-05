import prim

import matplotlib.pyplot as plt

from typing import List, Tuple

from matplotlib.animation import FuncAnimation
from matplotlib.axes import Axes
from matplotlib.figure import Figure
from matplotlib.ticker import MultipleLocator

class CIndex:
    def __init__(self, max_idx):
        self.idx = 0
        self.max_idx = max_idx

def fill_grid_with_color(ax: Axes, x_left_bottom, y_right_bottom, color = 'black'):
    x_line_points = [x_left_bottom, x_left_bottom + 1]
    y1 = y_right_bottom
    y2 = y_right_bottom + 1
    ax.fill_between(x_line_points, y1, y2, facecolor=color)

def show(context, draw_roads = False):
    X = context['X']
    Y = context['Y']
    fig: Figure = context['fig']
    ax: Axes = context['ax']
    pm: prim.PrimMaze = context['pm']
    roads: List[Tuple[int, int]] = context['roads']

    if not draw_roads:
        for row in range(pm.row_max):
            for col in range(pm.col_max):
                if pm.grids[row, col] == pm.WALL:
                    fill_grid_with_color(ax, col, row)
        plt.show()
        return

    # 黑色表示墙壁，一开始迷宫都是墙
    ax.fill_between([0, X], 0, Y, facecolor='black')

    cIndex = CIndex(len(roads))

    def update(frame):
        idx = cIndex.idx

        if idx >= cIndex.max_idx:
            return

        cIndex.idx += 1

        row, col = roads[idx]

        # 白色表示道路
        fill_grid_with_color(ax, col, row, 'white')

    anim = FuncAnimation(fig, update, frames=120, interval=50)

    plt.show()

def main():
    fig, ax = plt.subplots()

    X = 31
    Y = 31

    # 是否展示打通道路的过程
    draw_roads = False

    plt.xlim(0, X)
    plt.ylim(0, Y)

    ax.xaxis.set_major_locator(MultipleLocator(1))
    ax.yaxis.set_major_locator(MultipleLocator(1))

    ax.xaxis.grid(True, which='major')
    ax.yaxis.grid(True, which='major')

    ax.invert_yaxis()
    ax.xaxis.tick_top()

    pm = prim.PrimMaze(Y, X)
    roads = pm.generate()

    context = {
        'X': X,
        'Y': Y,
        'fig': fig,
        'ax': ax,
        'pm': pm,
        'roads': roads,
    }

    show(context, draw_roads)

if __name__ == '__main__':
    main()
