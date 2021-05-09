#pragma once

#include <SDL2/SDL.h>

class Renderer
{
public:

    Renderer(int, 
        const SDL_Color& on = { 0xff, 0xff, 0xff, 0xff }, 
        const SDL_Color& off = { 0, 0, 0, 0xff });
    ~Renderer();

    void draw();

private:
    
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Color _color[2];
    int _pxSize;
};