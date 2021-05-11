#include "cpu.h"
#include <iostream>

CPU* CPU::instance = nullptr;

CPU* CPU::get() 
{ return instance; }

CPU::CPU(bool& d):
    draw(d)
{ 
    instance = this; 
    load_font();
    match_opcodes();
}

CPU::~CPU()
{ instance = nullptr; }

void CPU::load_font()
{
    const int count = 5*0x10;

    Byte fonts[count] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80 
    };

    for (int i=0; i < count; ++i)
        memory[font_start + i] = fonts[i];
}

void CPU::match_opcodes()
{
    opcodes[0] = [this]{
        if (opcode & 0xf)
            o00EE();
        else
            o00E0();
    };
    opcodes[1] = [this]{
        o1NNN();
    };
    opcodes[2] = [this]{
        o2NNN();
    };
    opcodes[3] = [this]{
        o3XKK();
    };
    opcodes[4] = [this]{
        o4XKK();
    };
    opcodes[5] = [this]{
        o5XY0();
    };
    opcodes[6] = [this]{
        o6XKK();
    };
    opcodes[7] = [this]{
        o7XKK();
    };
    opcodes[8] = [this]{
        switch (opcode & 0xf)
        {
        case 0:
            o8XY0();
            break;
        case 1:
            o8XY1();
            break;
        case 2:
            o8XY2();
            break;
        case 3:
            o8XY3();
            break;
        case 4:
            o8XY4();
            break;
        case 5:
            o8XY5();
            break;
        case 6:
            o8XY6();
            break;
        case 7:
            o8XY7();
            break;
        case 0xE:
            o8XYE();
            break;
        default : ;
        };
    };
    opcodes[9] = [this]{
        o9XY0();
    };
    opcodes[0xA] = [this]{
        oANNN();
    };
    opcodes[0xB] = [this]{
        oBNNN();
    };
    opcodes[0xC] = [this]{
        oCXKK();
    };
    opcodes[0xD] = [this]{
        oDXYN();
    };
    opcodes[0xE] = [this]{
        if (opcode & 0xf == 1)
            oEXA1();
        else
            oEX9E();
    };
    opcodes[0xF] = [this]{
        switch(opcode & 0xf)
        {
        case 7:
            oFX07();
            break;
        case 0xA:
            oFX0A();
            break;
        case 5:
            switch((opcode & 0xf0) >> 4)
            {
            case 1:
                oFX15();
                break;
            case 5:
                oFX55();
                break;
            case 6:
                oFX65();
                break;
            default : ;
            };
            break;
        case 8:
            oFX18();
            break;
        case 0xE:
            oFX1E();
            break;
        case 9:
            oFX29();
            break;
        case 3:
            oFX33();
            break;
        default : ;
        };
    };
}

void CPU::process()
{
    std::cout << "PC     : " << std::hex << pc << std::endl;
    std::cout << "Opcode : " << std::hex << opcode << std::endl;

    opcode = (memory[pc] << 8) | memory[pc+1];
    pc += 2;

    opcodes[(opcode & 0xf000) >> 12]();

    if (delay > 0)
        delay--;
    if (sound > 0)
        sound--;
}