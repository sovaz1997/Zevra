#include "game.hpp"

int Game::startGame() {
	std::string str;
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
	
	int64_t current_alpha = -WHITE_WIN, current_beta = WHITE_WIN, win_size = 50;

	for(; max_depth <= max_depth_global; ++max_depth) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();
		bool fail;

		negamax(game_board, current_alpha, current_beta, max_depth, 0, FIXED_DEPTH, false, fail);


		if(fail) {
			current_alpha = -WHITE_WIN, current_beta = WHITE_WIN;
			--max_depth;
		} else {
			current_alpha = bestScore - win_size;
			current_beta = bestScore + win_size;
		}

		hasBestMove = true;

		if((abs(bestScore) >= (WHITE_WIN - 100) && max_depth_global < 99) || stopped) {
			break;
		}
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

	int64_t current_alpha = -WHITE_WIN, current_beta = WHITE_WIN, win_size = 50;

	for(; timer.getTime() < time; ) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		bool fail;
		//int stack = 0, stop = 6;
		negamax(game_board, current_alpha, current_beta, max_depth, 0, FIXED_TIME, false, fail);

		if(/*stack <= stop && */fail/*bestScore >= current_beta || bestScore <= current_alpha*/) {
			/*++stack;
			if(bestScore >= current_beta) {
				current_beta += win_size;
			}
			if(bestScore <= current_alpha) {
				current_alpha -= win_size;
			}*/
			current_alpha = -WHITE_WIN, current_beta = WHITE_WIN;
			--max_depth;
		} else {
			//stack = 0;
			current_alpha = bestScore - win_size;
			current_beta = bestScore + win_size;
		}

		//std::cout << current_alpha << " " << current_beta << "\n";

		if(abs(bestScore) >= (WHITE_WIN - 100) || stopped) {
			break;
		}

		

		hasBestMove = true;
		++max_depth;
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
