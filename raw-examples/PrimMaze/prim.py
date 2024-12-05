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

        self.grids = np.full((row_max, col_max), self.WALL)
        self.walls: List[Tuple[int, int]] = []
        self.directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

        self.LEFT = 1
        self.RIGHT = 2
        self.UP = 3
        self.DOWN = 4

    def isValidGrid(self, grid: Tuple[int, int]):
        row, col = grid
        cond1 = row > 0 and row < self.row_max - 1
        cond2 = col > 0 and col < self.col_max - 1
        return cond1 and cond2

    def addNeighbors(self, grid: Tuple[int, int]):
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
        self.grids[self.start_point] = self.ROAD

        self.addNeighbors(self.start_point)

        roads: List[Tuple[int, int]] = []
        roads.append(self.start_point)

        wallLen = len(self.walls)

        while wallLen > 0:
            idx = random.randint(0, wallLen - 1)
            wall = self.walls[idx]

            upNeighbor = self.getNeighbor(wall, self.UP)
            downNeighbor = self.getNeighbor(wall, self.DOWN)

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
                    roads.extend([upNeighbor, wall])

            leftNeighbor = self.getNeighbor(wall, self.LEFT)
            rightNeighbor = self.getNeighbor(wall, self.RIGHT)

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
                    roads.extend([leftNeighbor, wall])

            self.walls.remove(wall)

            wallLen = len(self.walls)

        return roads
