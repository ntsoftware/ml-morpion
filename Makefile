#all: morpion
#all: libboardwrapper.so
all: minimax

minimax: minimax.c
	$(CC) -Wall -Werror -O3 $< -o $@

libboardwrapper.so: boardwrapper.c boardwrapper.h
	$(CC) -g -Wall -Werror -shared $< -o $@

morpion: main.cpp board.h
	$(CXX) -g -Wall -Werror $< -o $@

clean:
	$(RM) -f morpion libboardwrapper.so minimax
	