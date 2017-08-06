CPP_FILES := $(wildcard ../*.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))
CXXFLAGS := -I.. -DEMULATED -Wall -fexec-charset=windows-1251 -std=c++11
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

emulator: main.cpp ${OBJ_FILES}
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

%.o: ../%.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

clean:
	rm -f emulator* ${OBJ_FILES}
	
run: emulator
	./emulator
	
.PHONY: clean run
