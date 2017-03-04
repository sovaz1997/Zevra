/*
  Zevra, a UCI chess playing engine
  Copyright (C) 2016-2017 Oleg Smirnov (author)
  Zevra is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
  Zevra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "game.hpp"

int Game::startGame() {
	std::string str;
	game_board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	while(true) {
		std::getline(std::cin, str);

		if(!uciHandler(str)) {
			return 0;
		};
	}
}

void Game::goFixedDepth() {
	clearCash();
	stopped = false;

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;
	int max_depth_global = max_depth;
	max_depth = 1;

	/*if(option.lmrEnable || option.nullMovePrunningEnable) {
		max_depth = 5;
	}*/

	start_timer = clock();
	hasBestMove = false;
	//double depth;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	hasBestMove = true;
	bestScore = 0;

	for(; max_depth <= max_depth_global; ++max_depth) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		//flattenHistory();

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false);
		//negamax_future(-WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, true, nullptr);
		hasBestMove = true;

		/*if((abs(bestScore) >= (WHITE_WIN - 100) && max_depth_global < 99) || stopped) {
			break;
		}*/
	}

	end_timer = clock();

	if(hasBestMove) {
		std::cout << "bestmove " << bestMove.getMoveString() << std::endl;
	}
}

void Game::goFixedTime(int tm) {
	stopped = false;
	if(tm >= 200) {
		tm -= 100;
	}

	time = tm;
	timer.start();

	clearCash();

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;

	start_timer = clock();
	hasBestMove = false;
	//double depth;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	bestScore = 0;
	hasBestMove = true;
	max_depth = 1;
	
	/*if(option.lmrEnable || option.nullMovePrunningEnable) {
		max_depth = 5;
	}*/
	
	std::vector<BitMove> bestPV;

	for(; timer.getTime() < time; ++max_depth) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		//flattenHistory();

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false);
		//negamax_future(-WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, true, nullptr);
		/*if(abs(bestScore) >= (WHITE_WIN - 100) || stopped) {
			break;
		}*/

		hasBestMove = true;
	}

	end_timer = clock();

	if(hasBestMove) {
		std::cout << "bestmove " << bestMove.getMoveString() << std::endl;
	}
}

void Game::goTournament() {
	double tm, inc;
	if(game_board.whiteMove) {
		tm = wtime;
		inc = winc;
	} else {
		tm = btime;
		inc = binc;
	}

	double k = 50;

	if(movestogoEnable) {
		k = movestogo;
		goFixedTime(tm / k + inc);
	} else {
		goFixedTime(tm / k + inc - 100);
	}
}

bool Game::move(std::string mv) {
	MoveArray moves;
	game_board.bitBoardMoveGenerator(moves);
	for(unsigned int i = 0; i < moves.count; ++i) {
		if(moves.moveArray[i].getMoveString() == mv) {
			game_board.move(moves.moveArray[i]);
			uint8_t color;

			if(game_board.whiteMove) {
				color = BLACK;
			} else {
				color = WHITE;
			}

			if(game_board.inCheck(color)) {
				game_board.goBack();
				continue;
			}

			++hash_decrement;
			return true;
		}
	}

	return false;
}
