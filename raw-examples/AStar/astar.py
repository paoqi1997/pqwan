import math

from typing import List, Tuple

from map import Map
from vec2 import DiagonalDistance, ManhattanDistance, Vec2

class AStar:
    def __init__(self, map: Map, diagonalMovementEnabled = True):
        self.open_list: List[Vec2] = []
        self.closed_list: List[Vec2] = []
        self.map = map
        # 是否允许对角线移动
        self.diagonalMovementEnabled = diagonalMovementEnabled
        self.initDirections()

        # 单位移动代价
        self.UNIT_COST1 = 1
        self.UNIT_COST2 = math.sqrt(2)

    def initDirections(self):
        self.directions: List[Vec2] = []
        self.directions.append(Vec2(1, 0))
        self.directions.append(Vec2(0, 1))
        self.directions.append(Vec2(-1, 0))
        self.directions.append(Vec2(0, -1))
        self.directions.append(Vec2(1, 1))
        self.directions.append(Vec2(1, -1))
        self.directions.append(Vec2(-1, 1))
        self.directions.append(Vec2(-1, -1))

    def planning(self) -> Tuple[List[Vec2], List[Vec2]]:
        self.open_list.append(self.map.start_point)

        path_points = []
        explored_points = []

        while True:
            index = self.selectPointInOpenList()
            if index < 0:
                break

            point = self.open_list[index]

            if not self.isInPointList(point, explored_points):
                explored_points.append(point)

            if self.isEndPoint(point):
                path_points = self.buildPath(point)
                break

            del self.open_list[index]

            self.closed_list.append(point)

            self.moveOn(point)

        return path_points, explored_points

    def selectPointInOpenList(self):
        min_cost = math.inf
        selected_index = -1

        for idx, point in enumerate(self.open_list):
            cost = self.totalCost(point)
            if cost < min_cost:
                min_cost = cost
                selected_index = idx

        return selected_index

    def moveOn(self, point: Vec2):
        for idx, direction in enumerate(self.directions):
            if not self.diagonalMovementEnabled and idx >= 4:
                break

            next_point = Vec2(point.x + direction.x, point.y + direction.y)
            step_cost = self.UNIT_COST1 if idx < 4 else self.UNIT_COST2

            self.step(point, next_point, step_cost)

    def step(self, point: Vec2, next_point: Vec2, step_cost):
        if not self.isValidPoint(next_point):
            return

        if self.isInClosedList(next_point):
            return

        current_cost = point.G + step_cost

        this_point = self.GetPoint(next_point.x, next_point.y, self.open_list)

        if this_point == None:
            next_point.G = current_cost
            next_point.parent = point
            self.open_list.append(next_point)
        # 找到更优的路径
        elif current_cost < this_point.G:
            p1_msg = f'this_point({this_point.G:.2f}): ({this_point.x}, {this_point.y})'
            p2_msg = f'point({current_cost:.2f}): ({point.x}, {point.y})'
            print(f'{p1_msg} -> {p2_msg}')
            this_point.G = current_cost
            this_point.parent = point

    def buildPath(self, point: Vec2):
        points = []

        while True:
            points.insert(0, point)
            if self.isStartPoint(point):
                break
            else:
                point = point.parent

        return points

    def heuristicCost(self, point: Vec2):
        '''h(n)'''
        if self.diagonalMovementEnabled:
            return DiagonalDistance(point, self.map.end_point)
        else:
            return ManhattanDistance(point, self.map.end_point)

    def totalCost(self, point: Vec2):
        '''f(n) = g(n) + h(n)'''
        return point.G + self.heuristicCost(point)

    def GetPoint(self, x, y, points: List[Vec2]):
        for p in points:
            if x == p.x and y == p.y:
                return p

        return None

    def isInPointList(self, point: Vec2, points: List[Vec2]):
        for p in points:
            if point.x == p.x and point.y == p.y:
                return True

        return False

    def isInOpenList(self, point: Vec2):
        return self.isInPointList(point, self.open_list)

    def isInClosedList(self, point: Vec2):
        return self.isInPointList(point, self.closed_list)

    def isStartPoint(self, point: Vec2):
        return point.x == self.map.start_point.x and point.y == self.map.start_point.y

    def isEndPoint(self, point: Vec2):
        return point.x == self.map.end_point.x and point.y == self.map.end_point.y

    def isValidPoint(self, point: Vec2):
        x, y = point.x, point.y

        if x < 0 or y < 0:
            return False

        if x >= self.map.x_max or y >= self.map.y_max:
            return False

        return not self.map.isObstacle(x, y)
