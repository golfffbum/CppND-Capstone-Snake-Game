#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  virtual void CheckChaos(Renderer &renderer, Uint32 frame_end, Uint32 &chaos_timestamp){return;}

 protected:
  virtual void Update();

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  Snake snake;

 private:
  void PlaceFood();
  void CheckForFood(int new_x, int new_y);
  void HandleInput(Controller const &controller, bool &running);


  SDL_Point food;
  int score{0};
};

#endif