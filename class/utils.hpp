#pragma once
#include <string>
#include "chips.hpp"

class GameMap{
  public:
    GameMap(std::string filename, class MessageBar*, int width, int height);
    void set(int x, int y, class Chip*);
    class Chip *get(int x, int y);
    void draw();
    void scroll_x(int x);
    void scroll_y(int y);
    static void map_init(std::string filename, int height, int width);
};

class MessageBar{
  public:
    MessageBar(class GameMap*, class Player*);
    std::string to_s();
};
