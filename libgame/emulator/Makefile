all:
	mkdir -p bin
	$(CXX) -DEMULATED -Wno-narrowing main.cpp ../*.cpp -lsfml-graphics -lsfml-window -lsfml-system -o bin/emulator -std=c++11
