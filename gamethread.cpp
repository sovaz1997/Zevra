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

int64_t Game::mtdf(int64_t f, int depth) {
   int64_t bound[2] = {-WHITE_WIN, +WHITE_WIN};
   do {
      double beta = f + (f == bound[0]);
      f = negamax(game_board, beta - 1, beta, depth, 0, FIXED_DEPTH, false, true);
      bound[f < beta] = f;
   } while (bound[0] < bound[1]);
   return f;
}

void Game::goFixedDepth() {
	//clearCash();
	++hashAge;
	stopped = false;

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;
	int max_depth_global = max_depth;
	max_depth = 1;

	start_timer = clock();
	hasBestMove = false;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	hasBestMove = true;
	bestScore = 0;
	int64_t f = 0;
	
	int64_t current_alpha = -WHITE_WIN, current_beta = WHITE_WIN, win_size = 50;

	for(; max_depth <= max_depth_global; ++max_depth) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		//negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);

		if(max_depth > 1) {
			current_alpha = bestScore - 30;
			current_beta = bestScore + 30;
		}

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);
		/*std::cout << "!";
		if(bestScore <= current_alpha) {
			std::cout << "!";
			negamax(game_board, -WHITE_WIN, current_beta, max_depth, 0, FIXED_DEPTH, false, true);
		} else if(bestScore >= current_beta) {
			std::cout << "!";
			negamax(game_board, current_alpha, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);
		}*/

		//f = mtdf(f, max_depth);

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
	/*if(tm >= 200) {
		tm -= 100;
	}*/

	time = tm;
	timer.start();

	//clearCash();
	++hashAge;

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;

	start_timer = clock();
	hasBestMove = false;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	bestScore = 0;
	hasBestMove = true;
	max_depth = 1;

	
	std::vector<BitMove> bestPV;

	for(; timer.getTime() < time; ) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true);
		
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
