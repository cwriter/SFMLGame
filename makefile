CC = /usr/bin/g++
CFlags = -g -mtune=native -std=c++11 -Iheaders
TMPDIR = ./obj/
LDFlags = -lsfml-system -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-window -pthread -lX11 -ldl

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

.PHONEY: clean


Game: $(OBJ_FILES)
	$(CC) $(LDFlags) -o $@ $^

clean:
	rm $(OBJ_FILES)

obj/%.o: src/%.cpp
	$(CC) $(CFlags) -c -o $@ $<
