#include "chip8.h"
#include "timer.h"
#include <iostream>
#include <ctime>

Chip_8::Chip_8(const std::string& rom) :
    running(true), draw(false),
    cpu(draw), renderer(10)
{
    load_rom (rom);

    bind = {
        { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 }, { SDLK_4, 0xc },
        { SDLK_a, 0x4 }, { SDLK_z, 0x5 }, { SDLK_e, 0x6 }, { SDLK_r, 0xd },
        { SDLK_q, 0x7 }, { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xe },
        { SDLK_w, 0xa }, { SDLK_x, 0x0 }, { SDLK_c, 0xb }, { SDLK_v, 0xf }
    };

    srand(time(0));
}

void Chip_8::run()
{
    Uint32 tick;
    Timer timer;
    const float rate = 1000.0/FPS;

    while (running)
    {
        timer.start();

        handle_event();
        cpu.process();
        
        if (draw)
        {
            draw = false;
            renderer.draw();
        }

        tick = timer.get();
        if (tick < rate)
            SDL_Delay(rate - tick);
    }
}

void Chip_8::load_rom(const std::string& rom) 
{
    FILE* file = fopen(rom.c_str(), "rb");

    if (!file)
    {
        // TODO : use messagebox instead
        std::cerr << "File not found!" << std::endl;
        exit(1);
    }

    fread(&cpu.memory[cpu.mem_start], 0xdff, 1, file);
    fclose(file);

}

void Chip_8::handle_event()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN)
        {
            SDL_Keycode scancode = event.key.keysym.sym;
            if (scancode == SDLK_ESCAPE)
                running = false;
            else if (bind.find(scancode) != bind.end())
                cpu.keys[bind[scancode]] = 1;
        }
        else if (event.type == SDL_KEYUP)
        {
            SDL_Keycode scancode = event.key.keysym.sym;
            if (bind.find(scancode) != bind.end())
                cpu.keys[bind[scancode]] = 0;
        }
    }
}