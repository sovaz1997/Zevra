CC = g++
CFLAGS = -Wall -pedantic -std=c++17 -m64 -O4 -flto -march=native -funroll-loops -msse

GDB = -std=c++17 -Wall -pedantic -m64 -O4 -flto -march=native -funroll-loops -pthread -g

GPROF = -g -pg

GCOV = -std=c++17 -Wall -pedantic -m64 -pthread -fprofile-arcs -ftest-coverage

FILES = constants.hpp main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o point.o uci.o evalute.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o timer.o



all: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o zevra
#gdb: constants.hpp main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o point.o uci.o evalute.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o
#	$(CC) $(GDB) main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o point.o  uci.o evalute.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o -o nalimov
gprof: $(FILES)
	$(CC) $(CFLAGS) $(GPROF) $(FILES) -o nalimov

#gcov: constants.hpp main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o point.o uci.o evalute.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o
#	$(CC) $(GCOV) main.o board.o game.o move.o movelist.o moveitem.o killer.o hash.o debute.o boardinfo.o point.o  uci.o evalute.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o -o nalimov


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

point.o: point.hpp point.cpp
	$(CC) $(CFLAGS) -c point.cpp

uci.o: game.hpp uci.cpp
	$(CC) $(CFLAGS) -c uci.cpp

evalute.o: game.hpp evalute.cpp
	$(CC) $(CFLAGS) -c evalute.cpp

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

clean:
	rm -rf *.o nalimov
