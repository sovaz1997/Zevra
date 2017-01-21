CC = g++
CFLAGS = -std=c++11 -m64 -Wall -pedantic -O4 -march=native -flto -funroll-loops
FILES = main.o game.o killer.o hash.o boardinfo.o point.o uci.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o timer.o goback.o figurecell.o bitmove.o movearray.o bitboard.o category.o magic.o option.o pv.o constants.o extendedmove.o eval.o
NAME = zevra

all: constants.hpp $(FILES)
	$(CC) $(CFLAGS) $(OPTIMIZATION) $(FILES) -o $(NAME)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

game.o: game.hpp game.cpp
	$(CC) $(CFLAGS) -c game.cpp

killer.o: killer.hpp killer.cpp
	$(CC) $(CFLAGS) -c killer.cpp

hash.o: hash.hpp hash.cpp
	$(CC) $(CFLAGS) -c hash.cpp

boardinfo.o: boardinfo.hpp boardinfo.cpp
	$(CC) $(CFLAGS) -c boardinfo.cpp

point.o: point.hpp point.cpp
	$(CC) $(CFLAGS) -c point.cpp

uci.o: game.hpp uci.cpp
	$(CC) $(CFLAGS) -c uci.cpp

preparation.o: game.hpp preparation.cpp
	$(CC) $(CFLAGS) -c preparation.cpp

printer.o: game.hpp printer.cpp
	$(CC) $(CFLAGS) -c printer.cpp

search.o: game.hpp search.cpp
	$(CC) $(CFLAGS) -c search.cpp

movegenerator.o: game.hpp movegenerator.cpp
	$(CC) $(CFLAGS) -c movegenerator.cpp

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

category.o: category.hpp category.cpp
	$(CC) $(CFLAGS) -c category.cpp

magic.o: magic.hpp magic.cpp
	$(CC) $(CFLAGS) -c magic.cpp

option.o: option.hpp option.cpp
	$(CC) $(CFLAGS) -c option.cpp

pv.o: pv.hpp pv.cpp
	$(CC) $(CFLAGS) -c pv.cpp

constants.o: constants.hpp constants.cpp
	$(CC) $(CFLAGS) -c constants.cpp

extendedmove.o: extendedmove.hpp extendedmove.cpp
	$(CC) $(CFLAGS) -c extendedmove.cpp

eval.o: eval.hpp eval.cpp
	$(CC) $(CFLAGS) -c eval.cpp

clean:
	rm -rf *.o $(NAME)
