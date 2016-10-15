#pragma once
#include <string>
#include "utils.hpp"

class Chip{
  public:
    Chip(class GameMap*);
    virtual void location(int x, int y) final;
    virtual std::string to_s();

    GameMap *game_map;
};

class Player : public Chip{
  using Chip::Chip;
  public:
    class Chip *get_top();
    class Chip *get_bottom();
    class Chip *get_left();
    class Chip *get_right();
    void move();
    void jump();
    std::string to_s();
};
