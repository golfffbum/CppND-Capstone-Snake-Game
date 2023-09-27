#include "menu.h"
#include <iostream>
#include "SDL.h"


void Menu::HandleInput(bool &waiting) {
  // Set game type based on user input
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      waiting = false;
    } 
    else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
        _game_type = GameType::classic;
        break;

        case SDLK_DOWN:
        _game_type = GameType::chaos;
        break;

        case SDLK_RETURN:
        waiting = false;
        break;
      }
    }
  }
}

