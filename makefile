SRC = chip8.cpp cpu.cpp opcodes.cpp renderer.cpp main.cpp

test : obj
	g++ obj/* -lSDL2 -o test
	
obj : $(SRC)
	@mkdir -p obj
	g++ -c $(SRC)
	@mv *.o obj/
