#include "gameChaos.h"
#include <iostream>
#include "SDL.h"

GameChaos::GameChaos(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height)
    :  Game(grid_width, grid_height) {

    _width_multiplier = screen_width/grid_width;
    _height_multiplier = screen_height/grid_height;
    obstacles.resize(15);

    for (auto& o : obstacles)
    {
      o.w = 2*_width_multiplier;
      o.h = 2*_height_multiplier;
    }
}

void GameChaos::CheckChaos(Renderer &renderer, Uint32 frame_end, Uint32 &chaos_timestamp) {

    // If snake is dead end chaos mode
    if (!snake.alive) {
      // If chaos mode is active turn it off 
      if (chaosModeActive)
      {
        chaosModeActive = false;
        renderer.TriggerObstacles(obstacles); //tbd
      }
      return;
    }
      
    // Check if it is time to enter chaos mode
    if (!chaosModeActive)
    {
      if (frame_end - chaos_timestamp >= _timeout)
      {
        chaos_timestamp = SDL_GetTicks();
        chaosModeActive = true;
        PlaceObstacles(renderer);
      }
    }
    else if (frame_end - chaos_timestamp >= _timein)
    {
      //Turn off chaos mode
      chaos_timestamp = SDL_GetTicks();
      chaosModeActive = false;
      _justHit = false; // reset hit timer
      renderer.TriggerObstacles(obstacles); // clear obstacles
      renderer.SetSnakeToRed(_justHit); // reset snake color
    }
    else
    {
      //change snake color when hit 
      renderer.SetSnakeToRed(_justHit);
    }
}

void GameChaos::PlaceObstacles(Renderer &renderer) {
  // generate obstacles and send them to renderer
  for (auto& obstacle : obstacles)
  {
    int x = random_w(engine);
    int y = random_h(engine);
    obstacle.x = x * obstacle.w/2;
    obstacle.y = y * obstacle.h/2;
  }

  renderer.TriggerObstacles(obstacles);
}

void GameChaos::Update() {
  // Call base class 
  Game::Update();

  if (!snake.alive) return;

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if obstacle is hit
  CheckForDamage(new_x, new_y);
}

void GameChaos::CheckForDamage(int new_x, int new_y) {
  Uint32 curr_time = SDL_GetTicks();

  if (chaosModeActive && !_justHit)
  {
    // Check if snake head is touching an obstacle
    for ( auto& obstacle : obstacles)
    {
      if (isPointInsideRect(new_x*_width_multiplier, new_y*_height_multiplier, obstacle))
      {
        // Obstacle is hit, decrease health
        snake.health = snake.health - 10;
        _justHit = true;
        _hitTimer = SDL_GetTicks();
        std::cout << "Ouch!\n";
      }
    }
  }
  else if(_justHit)
  {
    // Allow buffer from one hit to allow snake to move through obstacle
    if ((curr_time - _hitTimer) >= 500)
    {
      _justHit = false;
    }
  }
}

int GameChaos::GetHealth() const { return snake.health; }

bool GameChaos::isPointInsideRect(int x, int y, const SDL_Rect& rect) {
    return x >= rect.x && x < (rect.x + rect.w) &&
        y >= rect.y && y < (rect.y + rect.h);
}
