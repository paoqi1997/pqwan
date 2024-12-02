import math

class Vec2:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.G = 0
        self.parent: Vec2 = None

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
