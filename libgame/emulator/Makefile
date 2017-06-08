CPP_FILES := $(wildcard ../*.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))
CXXFLAGS := -DEMULATED -Wno-narrowing 

emulator: main.cpp ../*.cpp ../*.h
	$(CXX) -DEMULATED -Wall main.cpp ../*.cpp -lsfml-graphics -lsfml-window -lsfml-system -o emulator -std=c++11

clean:
	rm -f emulator*
	
run: emulator
	./emulator
	
.PHONY: clean run
