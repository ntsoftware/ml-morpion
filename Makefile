#all: morpion
all: libboardwrapper.so

libboardwrapper.so: boardwrapper.c boardwrapper.h
	$(CC) -g -Wall -Werror -shared $< -o $@

morpion: main.cpp board.h
	$(CXX) -g -Wall -Werror $< -o $@

clean:
	$(RM) -f morpion libboardwrapper.so
	