#  Zevra, a UCI chess playing engine
#  Copyright (C) 2016-2017 Oleg Smirnov (author)
#  Zevra is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  any later version.
#  Zevra is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = g++
CFLAGS = -std=c++11 -m64 -Wall -pedantic -O3 -flto -funroll-loops -march=native
FILES = main.o game.o killer.o hash.o uci.o preparation.o printer.o search.o gamethread.o gameservices.o movegenerator.o timer.o goback.o bitmove.o movearray.o bitboard.o magic.o option.o pv.o constants.o
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

pv.o: pv.hpp pv.cpp
	$(CC) $(CFLAGS) -c pv.cpp

constants.o: constants.hpp constants.cpp
	$(CC) $(CFLAGS) -c constants.cpp

clean:
	rm -rf *.o $(NAME)
