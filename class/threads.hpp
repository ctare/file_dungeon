#pragma once
#include <thread>

class Thread{
  public:
    virtual void start() final;
    virtual void run()=0;
    virtual void join() final;
    virtual void exit() final;
    bool isRun = true;

  private:
    std::thread thread;
};

class TimerThread : public Thread{
  public:
    TimerThread(int interval, void *(TimerThread*));
    virtual void run();
};

class GameThread : public Thread{
  public:
    GameThread(class GameMap*, class Player*);
    virtual void run();
    void draw();
};
