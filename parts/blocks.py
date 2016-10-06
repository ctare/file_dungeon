RELATIONAL = {}
from copy import deepcopy

def create_block(symbol):
    cls = RELATIONAL[symbol]
    return cls(cls)

class Anim:
    def __init__(self, *pattern):
        self.length = len(pattern)
        self.pattern = pattern
        self.now = 0
        self.n = 1

    def __str__(self):
        self.now += (1 / self.n)
        if self.now >= self.length: self.now = 0
        return str(self.pattern[int(self.now)])

    def magnification(self, n):
        self.n = n

class Block:
    def __init__(self, cls):
        self.symbol = deepcopy(cls.symbol)

    def __eq__(self, target):
        return isinstance(self, target)

    def __str__(self):
        return str(self.symbol)

    @classmethod
    def register(cls, symbol):
        cls.symbol = symbol
        RELATIONAL[cls.symbol] = cls
        return cls

    @classmethod
    def anim(cls, *pattern):
        cls.symbol = Anim(*pattern)
        return cls.symbol


class Wall(Block):
    pass


class Point(Block):
    def __str__(self):
        return str(self.symbol)


class Space(Block):
    pass
