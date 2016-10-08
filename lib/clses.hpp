#ifndef INCLUDE_CLSES
#define INCLUDE_CLSES

class GameMap;
class Player;
class GameThread;
class TimerThread;
class MessageBar;

class GameMap{
  public:
    GameMap(std::string filename, MessageBar *message_bar, int width, int height);
    void set(int x, int y, char val);
    char get(int x, int y);
    void draw();
    void scroll_x(int x);
    void scroll_y(int y);
    static void map_init(std::string filename, int height, int width);
};

class Player{
  public:
    Player(char appearance, GameMap *game_map);
    void location(int x, int y);
    char get_top();
    char get_bottom();
    char get_left();
    char get_right();
    void move();
    void jump();
    std::string to_s();
};

class GameThread{
  public:
    GameThread(GameMap *game_map, Player *player);
    void start();
    void run();
    void join();
    void draw();
    void exit();
};

class TimerThread{
  TimerThread(int interval, void (*fnc)(TimerThread *t_thread));
    void start();
    void run();
    void join();
    void exit();
};

class MessageBar{
  public:
    MessageBar(GameMap *game_map, Player *player);
    std::string to_s();
};

#endif
