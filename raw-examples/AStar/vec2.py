import math

from typing import Tuple

class Vec2:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        # g(n)
        self.G = 0
        # 父节点
        self.parent: Vec2 = None

    def updateXY(self, x, y):
        self.x = x
        self.y = y

    def fromTuple(self, t: Tuple[int, int]):
        self.x = t[0]
        self.y = t[1]

def ManhattanDistance(p1: Vec2, p2: Vec2):
    '''曼哈顿距离'''
    d = abs(p1.x - p2.x) + abs(p1.y - p2.y)
    return d

def DiagonalDistance(p1: Vec2, p2: Vec2):
    '''对角线距离'''
    dx = abs(p1.x - p2.x)
    dy = abs(p1.y - p2.y)
    d = dx + dy + (math.sqrt(2) - 2) * min(dx, dy)
    return d
