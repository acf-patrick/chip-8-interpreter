#include "chip8.h"
#include "timer.h"
#include <iostream>
#include <ctime>

Chip_8::Chip_8(const std::string& rom) :
    renderer(10), running(true)
{
    load_rom (rom);

    bind = {
        { SDL_SCANCODE_1, 0x1 }, { SDL_SCANCODE_2, 0x2 }, { SDL_SCANCODE_3, 0x3 }, { SDL_SCANCODE_4, 0xc },
        { SDL_SCANCODE_A, 0x4 }, { SDL_SCANCODE_Z, 0x5 }, { SDL_SCANCODE_E, 0x6 }, { SDL_SCANCODE_R, 0xd },
        { SDL_SCANCODE_Q, 0x7 }, { SDL_SCANCODE_S, 0x8 }, { SDL_SCANCODE_D, 0x9 }, { SDL_SCANCODE_F, 0xe },
        { SDL_SCANCODE_W, 0xa }, { SDL_SCANCODE_X, 0x0 }, { SDL_SCANCODE_C, 0xb }, { SDL_SCANCODE_V, 0xf }
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
        renderer.draw();

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

    fread(&cpu.memory[cpu.mem_start], 0x800, 1, file);
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
            SDL_Scancode scancode = event.key.keysym.scancode;
            if (scancode == SDL_SCANCODE_ESCAPE)
                running = false;
            else if (bind.find(scancode) != bind.end())
                cpu.keys[bind[scancode]] = 1;
        }
        else if (event.type == SDL_KEYUP)
        {
            SDL_Scancode scancode = event.key.keysym.scancode;
            if (bind.find(scancode) != bind.end())
                cpu.keys[bind[scancode]] = 0;
        }
    }
}