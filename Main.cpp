#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
using namespace std;

string wall = "#";
string point = "*";
string space = " ";

class GameMap{
}

class Player{
}

class GameThread{
}

class TimerThread{
}

class MessageBar{
}

int main(void){
  struct winsize term;
  ioctl(0, TIOCGWINSZ , &term);
  const int TERM_X = term.ws_col;
  const int TERM_Y = term.ws_row - 1;

  printf("%d, %d\n", TERM_X, TERM_Y);
}
