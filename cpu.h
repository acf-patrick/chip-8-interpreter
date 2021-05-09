#pragma once

#include <map>
#include <cstdint>
#include <functional>

using Byte = std::uint8_t;
using Word = std::uint16_t;

class Chip_8;
class Renderer;

class CPU
{
public:

    static CPU* get();

private:

    CPU(bool&);
    ~CPU();

    Byte memory[0x1000] = { 0x0 };  // 4kb
    Byte registers[0xf] = { 0x0 };  // V0... VF

    Word index = 0;                 // Special register
    Word pc = 0x200;                // program counter

    Word stack[0x10] = { 0x0 };
    Byte sp = 0;                    // stack pointer

    int keys[0x10] = {0};

    bool& draw;
    Byte display[64][32] = { {0x0} };

    Word opcode = 0x0;
    std::map<int, std::function<void(void)>> opcodes;

// Timers
    std::uint32_t delay, sound;

    static CPU* instance;
    
    const Word font_start = 0x50;
    const Word mem_start = 0x200;

    void match_opcodes();

    void load_font();

    void process();
    
private:

    void o00E0();
    void o00EE();
    void o1NNN();
    void o2NNN();
    void o3XKK();
    void o4XKK();
    void o5XY0();
    void o6XKK();
    void o7XKK();
    void o8XY0();
    void o8XY1();
    void o8XY2();
    void o8XY3();
    void o8XY4();
    void o8XY5();
    void o8XY6();
    void o8XY7();
    void o8XYE();
    void o9XY0();
    void oANNN();
    void oBNNN();
    void oCXKK();
    void oDXYN();
    void oEX9E();
    void oEXA1();
    void oFX07();
    void oFX0A();
    void oFX15();
    void oFX18();
    void oFX1E();
    void oFX29();
    void oFX33();
    void oFX55();
    void oFX65();

friend class Chip_8;
friend class Renderer;
};