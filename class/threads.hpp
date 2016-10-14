#pragma once
#include <thread>

class Thread{
  public:
    virtual void start() final{
      this -> thread = std::thread(&Thread::run, this);
    };
    virtual void run()=0;
    virtual void join() final{
      this -> thread.join();
    };
    virtual void exit() final{
      this -> isRun = false;
    };
    bool isRun = true;

  private:
    std::thread thread;
};

class TimerThread : public Thread{
  public:
    TimerThread(int interval, void (*fnc)(TimerThread *timer_thread));
    virtual void run();
};

class GameThread : public Thread{
  public:
    GameThread(class GameMap *game_map, class Player *player);
    virtual void run();
    void draw();
};
