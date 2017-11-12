PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

CC = g++
CFLAGS = -std=c++14 -m64 -Ofast -DNDEBUG -Wall -pedantic -flto -march=native -pthread
FILES = main.o game.o killer.o hash.o uci.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o timer.o goback.o figurecell.o bitmove.o movearray.o bitboard.o  magic.o option.o constants.o killers.o
NAME = zevra

all: constants.hpp $(FILES)
	$(CC) $(CFLAGS) $(OPTIMIZATION) $(FILES) -o $(NAME)

strip:
	strip $(NAME)

install:
	make -j4 clean
	make -j4 all
	-mkdir -p -m 755 $(BINDIR)
	-cp $(NAME) $(BINDIR)
	-strip $(BINDIR)/$(NAME)

uninstall:
	rm -rf $(BINDIR)/$(NAME)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

game.o: game.hpp game.cpp
	$(CC) $(CFLAGS) -c game.cpp

killer.o: killer.hpp killer.cpp
	$(CC) $(CFLAGS) -c killer.cpp

hash.o: hash.hpp hash.cpp
	$(CC) $(CFLAGS) -c hash.cpp

uci.o: game.hpp uci.cpp
	$(CC) $(CFLAGS) -c uci.cpp

preparation.o: game.hpp preparation.cpp
	$(CC) $(CFLAGS) -c preparation.cpp

printer.o: game.hpp printer.cpp
	$(CC) $(CFLAGS) -c printer.cpp

search.o: game.hpp search.cpp
	$(CC) $(CFLAGS) -c search.cpp

killers.o: killers.hpp killers.cpp
	$(CC) $(CFLAGS) -c killers.cpp

movegenerator.o: game.hpp movegenerator.cpp
	$(CC) $(CFLAGS) -c movegenerator.cpp

newbitmove.o: newbitmove.hpp newbitmove.cpp
	$(CC) $(CFLAGS) -c newbitmove.cpp

gamethread.o: game.hpp gamethread.cpp
	$(CC) $(CFLAGS) -c gamethread.cpp

gameservices.o: game.hpp gameservices.cpp
	$(CC) $(CFLAGS) -c gameservices.cpp

timer.o: timer.hpp timer.cpp
	$(CC) $(CFLAGS) -c timer.cpp

goback.o: timer.hpp goback.cpp
	$(CC) $(CFLAGS) -c goback.cpp

figurecell.o: figurecell.hpp figurecell.cpp
	$(CC) $(CFLAGS) -c figurecell.cpp

bitmove.o: bitmove.hpp bitmove.cpp
	$(CC) $(CFLAGS) -c bitmove.cpp

movearray.o: bitmove.hpp game.hpp movearray.hpp movearray.cpp
	$(CC) $(CFLAGS) -c movearray.cpp

bitboard.o: bitboard.hpp bitboard.cpp
	$(CC) $(CFLAGS) -c bitboard.cpp

magic.o: magic.hpp magic.cpp
	$(CC) $(CFLAGS) -c magic.cpp

option.o: option.hpp option.cpp
	$(CC) $(CFLAGS) -c option.cpp

constants.o: constants.hpp constants.cpp
	$(CC) $(CFLAGS) -c constants.cpp

clean:
	rm -rf *.o $(NAME)
