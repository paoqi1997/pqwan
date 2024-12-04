import sys
from os.path import dirname
sys.path.append(dirname(dirname(__file__)))

from PrimMaze import PrimMaze

from typing import List

from vec2 import Vec2

class Map:
    def __init__(self, x_max = 30, y_max = 30):
        self.obstacle_points: List[Vec2] = []
        self.x_max = x_max
        self.y_max = y_max
        self.start_point = Vec2(0, 0)
        self.end_point = Vec2(x_max - 1, y_max - 1)

    def isObstacle(self, x, y):
        for point in self.obstacle_points:
            if x == point.x and y == point.y:
                return True

        return False

    def addPoint(self, x, y):
        self.obstacle_points.append(Vec2(x, y))

    def generateObstacles(self, generateFunc = None):
        if generateFunc == None:
            self.generateObstacles_1()
        else:
            generateFunc()

    def generateFromPrimMaze(self):
        pm = PrimMaze(self.y_max, self.x_max)
        pm.generate()

        self.start_point = Vec2(pm.start_point[1], pm.start_point[0])
        self.end_point = Vec2(self.x_max - 2, self.y_max - 2)

        for row in range(pm.row_max):
            for col in range(pm.col_max):
                if pm.grids[row, col] == pm.WALL:
                    self.addPoint(col, row)

    def generateObstacles_1(self):
        x_left = round(self.x_max / 3)
        x_right = round(self.x_max * 2 / 3)

        y_bottom = round(self.y_max / 3)
        y_top = round(self.y_max * 2 / 3)

        for x in range(x_left, x_right):
            y = y_top
            self.addPoint(x, y)

        for y in range(y_bottom, y_top):
            x = x_right
            self.addPoint(x, y)

        self.addPoint(x_right, y_top)

    def generateObstacles_2(self):
        x_left = round(self.x_max / 3)
        x_right = round(self.x_max * 2 / 3)

        y_bottom = round(self.y_max / 10)
        y_top = round(self.y_max * 9 / 10)

        for y in range(y_bottom, self.y_max):
            x = x_left
            self.addPoint(x, y)

        for y in range(y_top):
            x = x_right
            self.addPoint(x, y)