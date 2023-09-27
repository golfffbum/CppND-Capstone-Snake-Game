#include <iostream>
#include "controller.h"
#include "game.h"
#include "gameChaos.h"
#include "renderer.h"
#include "menu.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Menu menu;
  bool waiting = true;

  //show menu and add delay so enter key doesnt stick 
  renderer.ShowMenu(menu.game_type());
  SDL_Delay(100);

  // Wait for player input
  while (waiting)
  {
    renderer.ShowMenu(menu.game_type());
    menu.HandleInput(waiting);
  }

  renderer.CloseMenu();

  // Run the game
  if (menu.game_type() == GameType::classic)
  {
    Game game(kGridWidth, kGridHeight);
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.GetScore() << "\n";
    std::cout << "Size: " << game.GetSize() << "\n";
  }
  else
  {
    GameChaos game(kGridWidth, kGridHeight, kScreenWidth, kScreenHeight);
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.GetScore() << "\n";
    std::cout << "Size: " << game.GetSize() << "\n";
  }

  return 0;
}