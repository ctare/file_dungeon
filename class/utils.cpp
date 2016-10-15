#include "utils.hpp"

GameMap::GameMap(std::string, class MessageBar *message_bar, int width, int height){
}

void GameMap::set(int x, int y, class Chip *chip){
}

class Chip *GameMap::get(int x, int y){
  return NULL;
}

void GameMap::draw(){
}

void GameMap::scroll_x(int x){
}

void GameMap::scroll_y(int y){
}

void GameMap::map_init(std::string filename, int height, int width){
}

MessageBar::MessageBar(class GameMap *game_map, class Player *player){
}

std::string MessageBar::to_s(){
  return NULL;
}
