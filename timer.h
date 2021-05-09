#pragma once

#include <SDL2/SDL.h>

class Timer
{
public:

    Timer()
    { start(); }

    void start()
    { start_t = SDL_GetTicks(); }

    Uint32 get()
    { return SDL_GetTicks() - start_t; }

private:

    Uint32 start_t;
};