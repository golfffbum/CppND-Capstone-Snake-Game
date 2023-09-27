#ifndef GAME_CHAOS_H
#define GAME_CHAOS_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "game.h"

class GameChaos : public Game {

 public:
  GameChaos(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height);

  void CheckChaos(Renderer &renderer, Uint32 frame_end, Uint32 &chaos_timestamp) override;
  int GetHealth() const;

 protected:
  void Update() override;

 private:
  void PlaceObstacles(Renderer &renderer);
  void CheckForDamage(int new_x, int new_y);
  bool isPointInsideRect(int x, int y, const SDL_Rect& rect);

  std::vector<SDL_Rect> obstacles;
  Uint32 _timeout{1000*5};
  Uint32 _timein{1000*5};
  bool chaosModeActive{false};
  std::size_t _width_multiplier;
  std::size_t _height_multiplier;
  bool _justHit{false};
  Uint32 _hitTimer{0};

};
#endif