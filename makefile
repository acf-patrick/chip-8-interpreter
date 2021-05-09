SRC = chip8.cpp cpu.cpp opcodes.cpp renderer.cpp main.cpp

test : obj
	g++ obj/* -lSDL2 -o chip-8
	
obj : $(SRC)
	@mkdir -p obj
	g++ -c $?
	@mv *.o obj/
