#!/usr/bin/python3
import threading
import time
import os
TERM_SIZE = os.get_terminal_size()
TERM_X = TERM_SIZE.columns
TERM_Y = TERM_SIZE.lines - 1

block = dict(
        wall="#",
        point="*",
        space=" ",
        )

class GameMap:
    def __init__(self, message_bar, width, height, filename=None):
        self.message_bar = message_bar
        self.origin_x = 0
        self.origin_y = 0
        self.width = width
        self.height = height - 1
        if filename == None:
            self.mapping = [[block["space"] for x in range(self.width)] for y in range(self.height)]
            self.size_x = self.width
            self.size_y = self.height
        else:
            self.mapping, self.size_x, self.size_y = self.map_init(filename)

    def set(self, x, y, val):
        self.mapping[y][x] = val

    def get(self, x, y):
        if x < 0 or self.size_x <= x or y < 0 or self.size_y <= y: return block["wall"]
        return self.mapping[y][x]

    def draw(self):
        print("[0;0H{}\n{}".format(
            self.message_bar,
            "\n".join(map(lambda x: "".join(map(str, x[self.origin_x:self.width + self.origin_x])), self.mapping[self.origin_y:self.height + self.origin_y]))), end="")

    def map_init(self, filename):
        with open(filename) as file_data:
            map_data = [line[:-1] for line in file_data]
        if self.height > len(map_data):
            map_data = ["" for x in range(self.height - len(map_data))] + map_data

        origin_x = max([len(x) for x in map_data]) + 2
        for index, line in enumerate(map_data):
            map_data[index] = list("{{:{}<{}}}".format(block["space"], origin_x).format(line))

        return map_data, origin_x - 1, len(map_data)

    def scroll_x(self, x):
        self.origin_x += x
        if self.origin_x < 0: self.origin_x = 0
        if self.origin_x >= self.size_x: self.origin_x = self.size_x - 1

    def scroll_y(self, y):
        self.origin_y += y
        if self.origin_y < 0: self.origin_y = 0
        if self.origin_y >= self.size_y: self.origin_y = self.size_y - 1

class Player:
    def __init__(self, appearance, game_map):
        self.appearance = appearance
        self.game_map = game_map
        self.x = None
        self.y = None
        self.ariel = True
        self.point = 0

    def location(self, x, y):
        self.game_map.set(x, y, self)
        self.x = x
        self.y = y

    def get_top(self):
        return self.game_map.get(self.x, self.y - 1)

    def get_bottom(self):
        return self.game_map.get(self.x, self.y + 1)

    def get_left(self):
        return self.game_map.get(self.x - 1, self.y)

    def get_right(self):
        return self.game_map.get(self.x + 1, self.y)

    def move(self, x, y):
        self.game_map.set(self.x, self.y, " ")
        self.x += x
        self.y += y
        now = self.game_map.get(self.x, self.y)
        if now == block["wall"]:
            self.x -= x
            self.y -= y
        elif now == block["point"]:
            self.point += 1
        self.game_map.set(self.x, self.y, self)

    def jump(self, high):
        if self.get_bottom() == block["wall"]:
            self.ariel = False
            def j(thread):
                if thread.count >= high or self.get_top() == block["wall"]:
                    self.ariel = True
                    thread.exit()
                self.move(0, -1)
            t = TimerThread(player_speed, j)
            t.start()



    def __str__(self):
        return str(self.appearance)


class GameThread(threading.Thread):
    def __init__(self, game_map, player):
        super().__init__()
        self.game_map = game_map
        self.not_end = True
        self.player = player

    def run(self):
        while self.not_end:
            self.draw()
            time.sleep(0.1)
            if self.player.x < self.game_map.origin_x or self.game_map.size_x <= self.game_map.origin_x:
                exit()

    def draw(self):
        self.game_map.draw()

    def exit(self):
        self.not_end = False

class TimerThread(threading.Thread):
    def __init__(self, interval, fnc):
        super().__init__()
        self.interval = interval
        self.not_end = True
        self.fnc = fnc
        self.count = 0

    def run(self):
        while self.not_end:
            time.sleep(self.interval)
            self.fnc(self)
            self.count += 1

    def exit(self):
        self.not_end = False

import sys

class MessageBar:
    def __init__(self, player, game_map):
        self.player = player

    def __str__(self):
        percentage = self.player.x / (self.game_map.size_x - 1) * 100
        view = "point: {} {:.1f}% ".format(self.player.point, percentage)
        if percentage < 100:
            progress_bar_len = TERM_X - len(view) - 3
            view += "|{{:{}}}".format(progress_bar_len).format("#" * int(percentage * progress_bar_len // 100)) + "|"
        else:
            view += "Game clear!!"
        return "{{:{}}}".format(TERM_X).format(view)

mb = MessageBar(None, None)
filename = sys.argv[1]
if not filename.endswith(".map"): filename += ".map"
mp = GameMap(mb, TERM_X, TERM_Y, filename)
p1 = Player("6", mp)
mb.player = p1
mb.game_map = mp
p1.location(1, mp.height - 2)

def frame(self):
    mp.scroll_x(1)
    p1.move(1, 0)
    if p1.ariel: p1.move(0, 1)

player_speed = 0.1
timeThread = TimerThread(player_speed, frame)
game = GameThread(mp, p1)
timeThread.start()
game.start()

def exit():
    game.exit()
    timeThread.exit()
    sys.exit("bye.")

while True:
    try:
        key = input()
    except:
        exit()
    if key == "":
        p1.jump(3)
