#include "threads.hpp"

void Thread::start(){
  this -> thread = std::thread(&Thread::run, this);
};

void Thread::join(){
  this -> thread.join();
};

void Thread::exit(){
  this -> isRun = false;
};

TimerThread::TimerThread(int interval, void *(fnc)(TimerThread *timer_thread)){
}

void TimerThread::run(){
}

GameThread::GameThread(class GameMap *game_map, class Player *player){
}

void GameThread::run(){
}

void GameThread::draw(){
}
