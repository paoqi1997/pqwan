import astar

import matplotlib.pyplot as plt

from functools import partial
from typing import List

from vec2 import Vec2

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

def update(frame, context):
    '''仅绘制最终路径'''
    cIndex: CIndex = context['c_index_path_points']

    idx = cIndex.idx

    if idx >= cIndex.max_idx:
        return

    cIndex.idx += 1

    ax: Axes = context['ax']
    astarAlgo: astar.AStar = context['astar_algo']
    path_points: List[Vec2] = context['path_points']

    point = path_points[idx]

    if not astarAlgo.isStartPoint(point) and not astarAlgo.isEndPoint(point):
        fill_grid_with_color(ax, point.x, point.y, 'green')

def updateWithExploredPoints(frame, context):
    '''绘制探索过的路径及最终路径'''
    ax: Axes = context['ax']
    cIndex1: CIndex = context['c_index_explored_points']
    cIndex2: CIndex = context['c_index_path_points']
    astarAlgo: astar.AStar = context['astar_algo']
    explored_points: List[Vec2] = context['explored_points']
    path_points: List[Vec2] = context['path_points']

    idx = cIndex1.idx

    if idx < cIndex1.max_idx:
        cIndex1.idx += 1

        point = explored_points[idx]

        if not astarAlgo.isStartPoint(point) and not astarAlgo.isEndPoint(point):
            fill_grid_with_color(ax, point.x, point.y, 'cyan')
    else:
        idx = cIndex2.idx

        if idx >= cIndex2.max_idx:
            return

        cIndex2.idx += 1

        point = path_points[idx]

        if not astarAlgo.isStartPoint(point) and not astarAlgo.isEndPoint(point):
            fill_grid_with_color(ax, point.x, point.y, 'green')

def main():
    X = 31
    Y = 31

    # 是否为随机迷宫
    is_maze = True
    # 是否显示探索过的路径
    with_explored_points = False

    fig, ax = preparePlot(X, Y, is_maze)

    map = astar.Map(X, Y)

    if is_maze:
        map.generateObstacles(map.generateFromPrimMaze)
    else:
        map.generateObstacles()

    # 绘制起点
    fill_grid_with_color(ax, map.start_point.x, map.start_point.y, 'blue')
    # 绘制终点
    fill_grid_with_color(ax, map.end_point.x, map.end_point.y, 'red')

    # 绘制障碍
    for point in map.obstacle_points:
        fill_grid_with_color(ax, point.x, point.y)

    # 迷宫地图不允许对角线移动
    diagonalMovementEnabled = False if is_maze else True

    astarAlgo = astar.AStar(map, diagonalMovementEnabled)
    path_points, explored_points = astarAlgo.planning()

    cIndex1 = CIndex(len(explored_points))
    cIndex2 = CIndex(len(path_points))

    context = {
        'ax': ax,
        'c_index_explored_points': cIndex1,
        'c_index_path_points': cIndex2,
        'astar_algo': astarAlgo,
        'explored_points': explored_points,
        'path_points': path_points,
    }

    updateFunc = updateWithExploredPoints if with_explored_points else update
    anim = FuncAnimation(fig, partial(updateFunc, context=context), frames=120, interval=50)

    plt.show()

if __name__ == '__main__':
    main()
