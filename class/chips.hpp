#pragma once
#include <string>

class Player{
  public:
    Player(char appearance, class GameMap *game_map);
    void location(int x, int y);
    char get_top();
    char get_bottom();
    char get_left();
    char get_right();
    void move();
    void jump();
    std::string to_s();
};
