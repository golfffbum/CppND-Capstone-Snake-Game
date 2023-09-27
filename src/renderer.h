#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <memory>
#include "SDL.h"
#include "SDL_ttf.h"

#include "snake.h"
#include "menu.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void ShowMenu(GameType type);

  void CloseMenu();
  void UpdateWindowTitle(int score, int fps, int health);
  void TriggerObstacles(std::vector<SDL_Rect> const &ob);
  void SetSnakeToRed(bool isInjured);


 private:
  SDL_Rect GenerateMessage(const std::string &text, const SDL_Color &color, TTF_Font *font, const int &x, const int &y);
  void DrawRectangle(GameType type);
  void RenderObstacle();
  void InvertColors();

  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  TTF_Font *_text_font_sm;
  TTF_Font *_text_font_bg;
  SDL_Rect _classic_rect;
  SDL_Rect _chaos_rect;
  bool _obstaclesOn{false};
  std::vector<SDL_Rect> _obstacles;
  SDL_Color _white;
  SDL_Color _red;
  SDL_Color _green;
  SDL_Color _blue;
  SDL_Color _yellow;
  SDL_Color _bg;
  SDL_Color _snake_color;
  Uint32 _invert_timer{0};
};

#endif