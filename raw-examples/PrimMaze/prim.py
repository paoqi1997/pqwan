import random

import numpy as np

from typing import List, Tuple

class PrimMaze:
    def __init__(self, row_max = 31, col_max = 31):
        self.WALL = 0
        self.ROAD = 1
        self.row_max = row_max
        self.col_max = col_max
        self.start_point = (1, 1)

        # 一开始迷宫都是墙
        self.grids = np.full((row_max, col_max), self.WALL)
        self.walls: List[Tuple[int, int]] = []
        self.directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

        self.LEFT = 1
        self.RIGHT = 2
        self.UP = 3
        self.DOWN = 4

    def isValidGrid(self, grid: Tuple[int, int]):
        row, col = grid
        # 迷宫的四周都有一堵墙
        cond1 = row > 0 and row < self.row_max - 1
        cond2 = col > 0 and col < self.col_max - 1
        return cond1 and cond2

    def addNeighbors(self, grid: Tuple[int, int]):
        '''将给定格子的周围四格放入墙壁列表'''
        curr_row, curr_col = grid

        for direction in self.directions:
            neighbor = (curr_row + direction[0], curr_col + direction[1])
            if self.isValidGrid(neighbor):
                self.walls.append(neighbor)

    def getNeighbor(self, grid: Tuple[int, int], direction):
        if direction == self.LEFT:
            return (grid[0], grid[1] - 1)
        elif direction == self.RIGHT:
            return (grid[0], grid[1] + 1)
        elif direction == self.UP:
            return (grid[0] - 1, grid[1])
        elif direction == self.DOWN:
            return (grid[0] + 1, grid[1])
        else:
            return None

    def generate(self):
        # 将起点纳入到道路中
        self.grids[self.start_point] = self.ROAD

        self.addNeighbors(self.start_point)

        roads: List[Tuple[int, int]] = []
        roads.append(self.start_point)

        wallLen = len(self.walls)

        while wallLen > 0:
            # 随机选择一堵墙
            idx = random.randint(0, wallLen - 1)
            wall = self.walls[idx]

            upNeighbor = self.getNeighbor(wall, self.UP)
            downNeighbor = self.getNeighbor(wall, self.DOWN)

            # 只要墙的一方是道路，另一方仍为墙壁，就把该墙和另一方对应的墙均拆除，使其成为迷宫的通路
            # 然后再将另一方的周围四格放入墙壁列表

            # 墙的上下方向
            if self.isValidGrid(upNeighbor) and self.isValidGrid(downNeighbor):
                if self.grids[upNeighbor] == self.ROAD and self.grids[downNeighbor] == self.WALL:
                    self.grids[wall] = self.ROAD
                    self.grids[downNeighbor] = self.ROAD
                    self.addNeighbors(downNeighbor)
                    roads.extend([wall, downNeighbor])

                if self.grids[upNeighbor] == self.WALL and self.grids[downNeighbor] == self.ROAD:
                    self.grids[upNeighbor] = self.ROAD
                    self.grids[wall] = self.ROAD
                    self.addNeighbors(upNeighbor)
                    roads.extend([wall, upNeighbor])

            leftNeighbor = self.getNeighbor(wall, self.LEFT)
            rightNeighbor = self.getNeighbor(wall, self.RIGHT)

            # 墙的左右方向
            if self.isValidGrid(leftNeighbor) and self.isValidGrid(rightNeighbor):
                if self.grids[leftNeighbor] == self.ROAD and self.grids[rightNeighbor] == self.WALL:
                    self.grids[wall] = self.ROAD
                    self.grids[rightNeighbor] = self.ROAD
                    self.addNeighbors(rightNeighbor)
                    roads.extend([wall, rightNeighbor])

                if self.grids[leftNeighbor] == self.WALL and self.grids[rightNeighbor] == self.ROAD:
                    self.grids[leftNeighbor] = self.ROAD
                    self.grids[wall] = self.ROAD
                    self.addNeighbors(leftNeighbor)
                    roads.extend([wall, leftNeighbor])

            # 访问过了，移除之
            self.walls.remove(wall)

            wallLen = len(self.walls)

        return roads
