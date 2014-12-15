import random
import math
import copy
from src.cro import CRO


class Test(CRO):
    """
    This is a derived class to find the minimum value of sin(x) + cos(y).
    In real terms, you should implement this class according to your real problem.
    Note that every copy operation is deep-copy.
    """
    def __init__(self, structure):
        CRO.__init__(self, structure)

    def wall(self, m):
        new = copy.deepcopy(m)
        new.structure[0], new.structure[1] = new.structure[1], new.structure[0]
        return new

    def dec(self, m):
        new1 = copy.deepcopy(m)
        new2 = copy.deepcopy(m)
        new1.structure[0] += random.random()
        new2.structure[1] += random.random()
        return [new1, new2]

    def inter(self, m1, m2):
        new1 = copy.deepcopy(m1)
        new2 = copy.deepcopy(m2)
        new1.structure[0] = m2.structure[0]
        new1.structure[1] = m1.structure[1]
        new2.structure[0] = m1.structure[0]
        new2.structure[1] = m2.structure[1]
        return [new1, new2]

    def syn(self, m1, m2):
        new = copy.deepcopy(m1)
        if random.random() < 0.5:
            new.structure[0] = m2.structure[0]
        else:
            new.structure[1] = m2.structure[1]
        return new

    def fit_func(self, m):
            return math.sin(m.structure[0]) + math.cos(m.structure[1])


# List of initial solutions: [[x1, y1], [x2, y2], [x3, y3]]
s = [[random.random(), random.random()] for i in range(10)]
cro = Test(s)
cro.run()
print cro.optimal.pe