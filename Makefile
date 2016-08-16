CC=g++
CFLAGS=-std=c++17 -Wall -pedantic -m64 -O4 -flto -march=native -funroll-loops -pthread

GDB=-std=c++17 -Wall -pedantic -m64 -O2 -g

all: constants.hpp main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o
	$(CC) $(CFLAGS) main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o -o tiger

gdb:  constants.hpp main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o
	$(CC) $(GDB) main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o -o tiger

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

board.o: board.hpp board.cpp
	$(CC) $(CFLAGS) -c board.cpp

game.o: game.cpp
	$(CC) $(CFLAGS) -c game.cpp

move.o: move.cpp
	$(CC) $(CFLAGS) -c move.cpp

movelist.o: movelist.hpp movelist.cpp
	$(CC) $(CFLAGS) -c movelist.cpp

moveitem.o: moveitem.hpp moveitem.cpp
	$(CC) $(CFLAGS) -c moveitem.cpp

killer.o: killer.hpp killer.cpp
	$(CC) $(CFLAGS) -c killer.cpp

hash.o: hash.hpp hash.cpp
	$(CC) $(CFLAGS) -c hash.cpp

debute.o: debute.hpp debute.cpp
	$(CC) $(CFLAGS) -c debute.cpp

	boardinfo.o: boardinfo.hpp boardinfo.cpp
		$(CC) $(CFLAGS) -c boardinfo.cpp

clean:
	rm -rf *.o tiger
