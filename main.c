#include "img.h"
#include "fractals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>

int main(int argc, char **argv) {
  pt p1 = {-2, -1.2};
  pt p2 = {1, 1.2};
  im* img = render_fractal(p1, p2, 1000, 0);
  save("test.ppm", img);

  unsigned char play = 1;
  SDL_Event event;
  SDL_Init(SDL_INIT_VIDEO);

  int windowWidth = 700;
  int windowHeight = 450;
  
  SDL_Window *window = SDL_CreateWindow("SDL2 Displaying Image",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RWops* rwop = SDL_RWFromFile("test.ppm", "rb");
  SDL_Surface *surface = IMG_LoadPNM_RW(rwop);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect srcRect;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = 1000;
  srcRect.h = 800;  
  float zoomFactor = 1.0f;
  float zoomStep = 1.004f;

  unsigned char zooming = 0;

  
while (play) {
    if (zooming)
      zoomFactor *= zoomStep;


    if (zoomFactor > 2) {
      zoomFactor = 1;

      long double ratioX = (p2.x - p1.x) / 1000;
      long double ratioY = (p2.y - p1.y) / 800;

      // Ajuster p1 et p2 en tenant compte de leur position initiale
      p1.x = p1.x + (srcRect.x * ratioX);
      p1.y = p1.y + (srcRect.y * ratioY);
      p2.x = p1.x + (srcRect.w * ratioX);
      p2.y = p1.y + (srcRect.h * ratioY);
      printf("(%Lf, %Lf) (%Lf, %Lf)\n", p1.x, p1.y, p2.x, p2.y);

      
      img = render_fractal(p1, p2, 1000, 0);
      save("test.ppm", img);
      SDL_RWclose(rwop);
      rwop = SDL_RWFromFile("test.ppm", "rb");
      SDL_DestroyTexture(texture);
      surface = IMG_LoadPNM_RW(rwop);
      texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);
      srcRect.w = 1000; srcRect.h = 800;
      srcRect.x = 0;
      srcRect.y = 0;
    }
    
    pt center;
    center.x = srcRect.x+srcRect.w / 2;
    center.y = srcRect.y+srcRect.h / 2;

    srcRect.w = 1000 / zoomFactor;
    srcRect.h = 800 / zoomFactor;
    
    srcRect.x = center.x - srcRect.w / 2;
    srcRect.y = center.y - srcRect.h / 2;


    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &srcRect, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    while (SDL_PollEvent(&event)) {
      double v;
      switch (event.type) {
        case SDL_QUIT:
          play = 0;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_UP:
              v = srcRect.y - 10/zoomFactor;
              if (v > 0)
                srcRect.y = v;
              break;
            case SDLK_DOWN:
              v = srcRect.y + 10/zoomFactor;
              if (v+srcRect.h < 800)
                srcRect.y = v;
              break;
            case SDLK_LEFT:
              v = srcRect.x - 10/zoomFactor;
              if (v > 0)
                srcRect.x = v;
              break;
            case SDLK_RIGHT:
              v = srcRect.x + 10/zoomFactor;
              if (v+srcRect.w < 1000)
                srcRect.x = v;
              break;
            case SDLK_SPACE:
              if (zooming == 0)
                zooming = 1;
              else
                zooming = 0;
              break;
          }
          break;
      }
    }
  }
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_RWclose(rwop);
  SDL_Quit();
  return 0;
}
