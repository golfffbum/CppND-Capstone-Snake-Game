#ifndef MENU_H
#define MENU_H

#include <random>
#include "SDL.h"

enum GameType
{
    classic = 0,
    chaos = 1
};

class  Menu {
 public:
  void HandleInput(bool &waiting);

  GameType game_type(){return _game_type;}

 private:
    GameType _game_type{classic};
  
};

#endif