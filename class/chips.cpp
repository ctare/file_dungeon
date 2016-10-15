#include "chips.hpp"

Chip::Chip(class GameMap *game_map){
}

void Chip::location(int x, int y){
  game_map -> set(x, y, this);
}

std::string Chip::to_s(){
  return NULL;
}

class Chip *Player::get_top(){
  return NULL;
}

class Chip *Player::get_bottom(){
  return NULL;
}

class Chip *Player::get_left(){
  return NULL;
}

class Chip *Player::get_right(){
  return NULL;
}

void Player::move(){
}

void Player::jump(){
}

std::string Player::to_s(){
  return NULL;
}
