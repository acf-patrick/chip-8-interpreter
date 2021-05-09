#include "chip8.h"

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        std::cerr << "Veuillez spécifier un ROM à charger!" << std::endl;
        std::cerr << "ex : -$ chip-8 tetris" std::endl;
        return 1;
    }
    
    Chip_8(argv[1]).run();
    return 0;
}