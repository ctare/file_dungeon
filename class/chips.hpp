#pragma once
#include <string>

class Chip{
  public:
    Chip(class GameMap*);
    virtual void location(int x, int y) final{
      game_map -> set(x, y, this);
    }
    virtual std::string to_s();

    GameMap *game_map;
};

class Player{
  using Chip::Chip;
  public:
    void location(int x, int y);
    char get_top();
    char get_bottom();
    char get_left();
    char get_right();
    void move();
    void jump();
    std::string to_s();
};
