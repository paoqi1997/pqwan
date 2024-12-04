import astar

import matplotlib.pyplot as plt

from functools import partial

from matplotlib.animation import FuncAnimation
from matplotlib.axes import Axes
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

def preparePlot(x_max = 31, y_max = 31, is_maze = False):
    fig, ax = plt.subplots()

    plt.xlim(0, x_max)
    plt.ylim(0, y_max)

    ax.xaxis.set_major_locator(MultipleLocator(1))
    ax.yaxis.set_major_locator(MultipleLocator(1))

    ax.xaxis.grid(True, which='major')
    ax.yaxis.grid(True, which='major')

    if is_maze:
        ax.invert_yaxis()
        ax.xaxis.tick_top()

    return fig, ax

def main():
    X = 31
    Y = 31

    is_maze = True

    fig, ax = preparePlot(X, Y, is_maze)

    map = astar.Map(X, Y)

    if is_maze:
        map.generateObstacles(map.generateFromPrimMaze)
    else:
        map.generateObstacles()

    fill_grid_with_color(ax, map.start_point.x, map.start_point.y, 'blue')
    fill_grid_with_color(ax, map.end_point.x, map.end_point.y, 'red')

    for point in map.obstacle_points:
        fill_grid_with_color(ax, point.x, point.y)

    diagonalMovementEnabled = False if is_maze else True
    astarAlgo = astar.AStar(map, diagonalMovementEnabled)
    path_points, explored_points = astarAlgo.planning()

    cIndex1 = CIndex(len(explored_points))
    cIndex2 = CIndex(len(path_points))

    def update(frame, cIndex1: CIndex, cIndex2: CIndex):
        idx = cIndex1.idx
        cIndex1.idx += 1

        if idx < cIndex1.max_idx:
            point = explored_points[idx]

            if not astarAlgo.isStartPoint(point) and not astarAlgo.isEndPoint(point):
                fill_grid_with_color(ax, point.x, point.y, 'cyan')
        else:
            idx = cIndex2.idx
            cIndex2.idx += 1

            if idx >= cIndex2.max_idx:
                return

            point = path_points[idx]

            if not astarAlgo.isStartPoint(point) and not astarAlgo.isEndPoint(point):
                fill_grid_with_color(ax, point.x, point.y, 'green')

    anim = FuncAnimation(fig, partial(update, cIndex1=cIndex1, cIndex2=cIndex2), frames=120, interval=50)

    plt.show()

if __name__ == '__main__':
    main()
