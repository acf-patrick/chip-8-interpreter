#include "cpu.h"
#include <cstdlib>

void CPU::o00E0()
{
    for (int i=0; i<64; ++i)
        for (int j=0; j<32; ++j)
            display[i][j] = 0;

    draw = true;
}

void CPU::o00EE()
{
    pc = stack[--sp];
}

void CPU::o1NNN()
{
    pc = opcode & 0xfff;
}

void CPU::o2NNN()
{
    Word address = opcode & 0xfff;
    stack[sp] = pc;
    pc = address;
    sp++;
}

void CPU::o3XKK()
{
    int x = (opcode & 0xf00) >> 8,
        kk = opcode & 0xff;
    if (registers[x] == kk)
        pc += 2;
}

void CPU::o4XKK()
{
    int x = (opcode & 0xf00) >> 8,
        kk = opcode & 0xff;
    if (registers[x] != kk)
        pc += 2;
}

void CPU::o5XY0()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;
    if (registers[x] == registers[y])
        pc += 2;
}

void CPU::o6XKK()
{
    int x = (opcode & 0xf00) >> 8,
        kk = opcode & 0xff;
    registers[x] = kk;
}

void CPU::o7XKK()
{
    int x = (opcode & 0xf00) >> 8,
        kk = opcode & 0xff;
    registers[x] += kk;
}

void CPU::o8XY0()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;
    registers[x] = registers[y];
}

void CPU::o8XY1()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;
    registers[x] |= registers[y];
}

void CPU::o8XY2()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;
    registers[x] &= registers[y];
}

void CPU::o8XY3()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;
    registers[x] ^= registers[y];
}

void CPU::o8XY4()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;

    Word result = registers[x] + registers[y];
    registers[0xf] = result > 0xff;
    registers[x] = result & 0xff;
}

void CPU::o8XY5()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;

    registers[0xf] = registers[x] >= registers[y];
    registers[x] = registers[x] - registers[y];
}

void CPU::o8XY6()
{
    int x = (opcode & 0xf00) >> 8;
    registers[0xf] = registers[x] & 0x1;

    registers[x] >>= 1;
}

void CPU::o8XY7()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;

    registers[0xf] = registers[x] <= registers[y];
    registers[x] = registers[y] - registers[x];
}

void CPU::o8XYE()
{
    int x = (opcode & 0xf00) >> 8;

    registers[0xf] = registers[x] & 0x80 >> 7;
    registers[x] <<=2;
}

void CPU::o9XY0()
{
    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;

    if (registers[x] != registers[y])
        pc += 2;
}

void CPU::oANNN()
{
    index = opcode & 0xfff;
}

void CPU::oBNNN()
{
    int nnn = opcode & 0xfff;
    pc = nnn + registers[0];
}

void CPU::oCXKK()
{
    int x = (opcode & 0xf00) >> 8,
        kk = opcode & 0xff;
    
    registers[x] = (rand()%0x1000) & kk;
}

void CPU::oDXYN()
{
    int width = 8,
        height = opcode & 0xf;

    int x = (opcode & 0xf00) >> 8,
        y = (opcode & 0xf0)  >> 4;

    if (!height)
        width = height = 16;

    Byte pos[2] = {
        registers[x] % 64,
        registers[y] % 32
    };

    registers[0xf] = 0;

    for (int i=0; i < height; ++i)
    {
        Byte sprite = memory[index + i];
        for (int j=0; j < width; ++j)
        {
            Byte pixel = sprite & (0x80 >> j);
            if (pixel)
            {
                Byte& screenpx = display[pos[0]+j][pos[1]+i];
                if (screenpx == 1)
                    registers[0xf] = 1;
                screenpx ^= 1;
            }
        }
    }

    draw = true;
}

void CPU::oEX9E()
{
    int i = registers[(opcode & 0xf00) >> 8];

    if (keys[i])
        pc += 2;
}

void CPU::oEXA1()
{
    int i = registers[(opcode & 0xf00) >> 8];

    if (!keys[i])
        pc += 2;
}

void CPU::oFX07()
{
    int x = (opcode & 0xf00) >> 8;

    registers[x] = delay;
}

void CPU::oFX0A()
{
    int x = (opcode & 0xf00) >> 8;

    int i;
    for (i=0; i < 0x10; ++i)
        if (keys[i])
        {
            registers[x] = i;
            break;
        }

    if (i == 0x10)
        pc -= 2;
}

void CPU::oFX15()
{
    delay = registers[(opcode & 0xf00) >> 8];
}

void CPU::oFX18()
{
    sound = registers[(opcode & 0xf00) >> 8];
}

void CPU::oFX1E()
{
    index += registers[(opcode & 0xf00) >> 8]; 
}

void CPU::oFX29()
{
    Byte addr = registers[(opcode & 0xf00) >> 8];
    index = font_start + 5*addr;
}

void CPU::oFX33()
{
    int value = registers[(opcode & 0xf00) >> 8];

    for (int i=2; i >= 0; --i)
    {
        memory[index + i] = value%10;
        value /= 10;
    }
}

void CPU::oFX55()
{
    int x = (opcode & 0xf00) >> 8;

    for (int i=0; i <= x; ++i)
        memory[index + i] = registers[i];
}

void CPU::oFX65()
{
    int x = (opcode & 0xf00) >> 8;

    for (int i=0; i <= x; ++i)
        registers[i] = memory[index + i];
}