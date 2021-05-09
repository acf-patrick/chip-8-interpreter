#include "renderer.h"
#include "cpu.h"

#include <iostream>

Renderer::Renderer(int pxSize, const SDL_Color& on, const SDL_Color& off) : 
    _pxSize(pxSize)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(int(64*pxSize), int(32*pxSize), SDL_WINDOW_SHOWN, &_window, &_renderer);

    if (!_window)
    {
        std::cerr << "Unable to create the main window :" << std::endl;
        std::cerr << SDL_GetError();
        exit(1);
    }

    _color[0] = off;
    _color[1] = on;
}

Renderer::~Renderer()
{
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    SDL_Quit();
}

void Renderer::draw()
{
    SDL_SetRenderDrawColor(_renderer, _color[0].r, _color[0].g, _color[0].b, _color[0].a);
    SDL_RenderClear(_renderer);

    SDL_SetRenderDrawColor(_renderer, _color[1].r, _color[1].g, _color[1].b, _color[1].a);
    auto display = CPU::get()->display;
    SDL_Rect rect;

    for (int i=0; i < 32; ++i)
        for (int j=0; j < 64; ++j)
        {
            rect = {
                j*_pxSize, i*_pxSize,
                _pxSize, _pxSize
            };
            if (display[j][i])
                SDL_RenderFillRect(_renderer, &rect);
        }

    SDL_RenderPresent(_renderer);
}