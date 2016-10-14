#pragma once
#include <string>

class GameMap{
  public:
    GameMap(std::string filename, class MessageBar *message_bar, int width, int height);
    void set(int x, int y, char val);
    char get(int x, int y);
    void draw();
    void scroll_x(int x);
    void scroll_y(int y);
    static void map_init(std::string filename, int height, int width);
};

class MessageBar{
  public:
    MessageBar(class GameMap *game_map, class Player *player);
    std::string to_s();
};
