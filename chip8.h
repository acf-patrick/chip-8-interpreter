#pragma once

#include "cpu.h"
#include "renderer.h"

#include <map>
#include <string>

class Chip_8
{
public:

    Chip_8(const std::string&);
    void run();

private:

    void load_rom(const std::string&);

    void handle_event();

    bool running;
    bool draw;

    CPU cpu;
    Renderer renderer;

    SDL_Event event;

    std::map<SDL_Keycode, int> bind;
    std::map<SDL_Keycode, bool> keys;
    
    const int FPS = 60;
};