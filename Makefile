all: morpion

morpion: main.cpp board.h
	$(CXX) -g -Wall -Werror $< -o $@

clean:
	$(RM) -f morpion
	