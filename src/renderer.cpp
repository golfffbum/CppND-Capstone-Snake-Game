#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

   // init ttf
  if( TTF_Init() == -1 )
  {
    std::cerr << "SDL_ttf err: " << TTF_GetError() << "\n";
  }

  _text_font_sm = TTF_OpenFont("./font/PressStart2P-Regular.ttf", 10);
  _text_font_bg = TTF_OpenFont("./font/PressStart2P-Regular.ttf", 25);
      if (!_text_font_sm || !_text_font_bg) 
    {
        std::cerr << "Error loading ttf font.\n";
        return;
    }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize colors 
   _white = {0xFF, 0xFF, 0xFF, 0xFF};
  _red = {0xFF, 0x00, 0x00, 0xFF};
  _green = {0, 128, 0, 255};
  _blue = {0x00, 0x7A, 0xCC, 0xFF};
  _yellow = {0xFF, 0xCC, 0x00, 0xFF};
  _bg = {0x1E, 0x1E, 0x1E, 0xFF};
  _snake_color = _white;

}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::ShowMenu(GameType type) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, _bg.r, _bg.g, _bg.b, _bg.a);
  SDL_RenderClear(sdl_renderer);

  // Create messages
  std::string instructions = "Select your version and press enter";
  GenerateMessage(instructions, _white, _text_font_sm, screen_width/5, screen_height/8);
  _classic_rect = GenerateMessage("classic", _white, _text_font_bg, screen_width/3, screen_height/4);
  _chaos_rect = GenerateMessage("chaos", _white, _text_font_bg, screen_width/2.75, screen_height/2);

  // Show user which mode is selected
  DrawRectangle(type);

  // Update screen
  SDL_RenderPresent(sdl_renderer);
}

SDL_Rect Renderer::GenerateMessage(const std::string &text, const SDL_Color &color, TTF_Font *font, const int &x, const int &y)
{
  // Create a surface
  SDL_Surface* msg_surface =
    TTF_RenderText_Solid(font, text.c_str(), color);
  
  // Convert surface into message
  SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(sdl_renderer, msg_surface);
  SDL_Rect textRect = { x, y, msg_surface->w, msg_surface->h };

  // render the message
  SDL_RenderCopy(sdl_renderer, msg_texture, nullptr, &textRect);
  
  // Free up space
  SDL_DestroyTexture(msg_texture);
  SDL_FreeSurface(msg_surface);

  // Create rectagle for outer selection
  int offset = 10;
  SDL_Rect selectRect = { x - offset, y - offset, msg_surface->w + 2*offset, msg_surface->h + 2*offset};

  return selectRect;
}

void Renderer::CloseMenu() {
    //Close the font that was used
    TTF_CloseFont( _text_font_sm );
    TTF_CloseFont( _text_font_bg );
    
    //Quit SDL_ttf
    TTF_Quit();
  return;
}

void Renderer::DrawRectangle(GameType type) {
  SDL_Rect rect; 
  if (GameType::classic == type)
  {
    rect = _classic_rect;
  }
  else
  {
    rect = _chaos_rect;
    GenerateMessage("You have been warned", _white, _text_font_sm, screen_width/3, screen_height/1.5);
  }

  SDL_SetRenderDrawColor(sdl_renderer, _green.r, _green.g, _green.b, _green.a);

  SDL_RenderDrawRect(sdl_renderer, &rect);
  SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, _bg.r, _bg.g, _bg.b, _bg.a);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, _yellow.r, _yellow.g, _yellow.b, _yellow.a);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, _snake_color.r, _snake_color.g, _snake_color.b, _snake_color.a);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, _blue.r, _blue.g, _blue.b, _blue.a);

  } else {
    SDL_SetRenderDrawColor(sdl_renderer, _red.r, _red.g, _red.b, _red.a);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  if(_obstaclesOn)
  {
    RenderObstacle();

    // Control rate at which colors invert
    if (SDL_GetTicks() - _invert_timer >= 200)
    {
      InvertColors();
      _invert_timer = SDL_GetTicks();
    }
  }
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::TriggerObstacles(std::vector<SDL_Rect> const &ob)
{
  _obstaclesOn = !_obstaclesOn;
  _obstacles = ob;

  InvertColors();
}

void Renderer::RenderObstacle() {
  // Render obstacles 
  for (auto &obstacle : _obstacles)
  {
    SDL_SetRenderDrawColor(sdl_renderer, _white.r, _white.g, _white.b, _white.a);
    SDL_RenderFillRect(sdl_renderer, &obstacle);
  }
}

void Renderer::UpdateWindowTitle(int score, int fps, int health) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps) + " Health: " + std::to_string(health)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::InvertColors()
{
  // Switch white and background colors
  SDL_Color savedW = _white;

  _white = _bg;
  _bg = savedW;
}

void Renderer::SetSnakeToRed(bool isInjured) {
  if(isInjured)
  {
    _snake_color = _red;
  }
  else
  {
    _snake_color = _white;
  }
}
 