#include "chip8.h"

int main(int argc, char** argv)
{
    Chip_8("rom/TETRIS").run();
    return 0;
}