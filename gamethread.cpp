#include "game.hpp"

int Game::startGame() {
	std::string str;
	uciHandler("position startpos");	

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
      //f = negamax(game_board, beta - 1, beta, depth, 0, FIXED_DEPTH, false, true);
      bound[f < beta] = f;
   } while (bound[0] < bound[1]);
   return f;
}

void Game::goFixedDepth() {
	if(option.UCI_AnalyseMode || abs(bestScore) >= (WHITE_WIN - 100)) {
		clearCash();
	}
	
	stopped = false;

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;
	int max_depth_global = max_depth;
	max_depth = 1;

	//start_timer = clock();
	timer.start();
	hasBestMove = false;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	hasBestMove = true;
	bestScore = 0;
	//int64_t f = 0;
	
	//int64_t current_alpha = -WHITE_WIN, current_beta = WHITE_WIN, win_size = 50;

	BitBoard parallel_board[option.threads_count];

	for(int i = 0; i < option.threads_count; ++i) {
		parallel_board[i] = game_board;
	}


	for(; max_depth <= max_depth_global; ++max_depth) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		//negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);

		//if(max_depth > 1) {
		//	current_alpha = bestScore - 30;
		//	current_beta = bestScore + 30;
		//}

		//negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);

		std::vector<Killers> killers(option.threads_count);
		moveArray.resize(option.threads_count);
		for(int i = 0; i < moveArray.size(); ++i) {
			moveArray[i].resize(200);
		}

		std::thread thread[option.threads_count];
		bool mainThread = true;
		for(int i = 0; i < option.threads_count; ++i) {
			thread[i] = std::thread(&Game::negamax, this, std::ref(parallel_board[i]), std::ref(moveArray[i]), -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true, i/*mainThread*/, std::ref(killers[i]));
			mainThread = false;
			//negamax(parallel_board[i], moveArray[i], -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true);
			
		}

		for(int i = 0; i < option.threads_count; ++i) {
			thread[i].join();	
		}

		//std::cout << "!";
		//if(bestScore <= current_alpha) {
		//	std::cout << "!";
		//	negamax(game_board, -WHITE_WIN, current_beta, max_depth, 0, FIXED_DEPTH, false, true);
		//} else if(bestScore >= current_beta) {
		//	std::cout << "!";
		//	negamax(game_board, current_alpha, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);
		//}

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

void Game::goFixedTime(int tm, bool tournamentTimeControll) {
	stopped = false;
	/*if(tm >= 200) {
		tm -= 100;
	}*/

	time = tm;
	timer.start();

	if(option.UCI_AnalyseMode || abs(bestScore) >= (WHITE_WIN - 100)) {
		clearCash();
	}

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

	/*for(; timer.getTime() < time; ) {
		
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		if(tournamentTimeControll) {
			if(timer.getTime() * 2 >= time) {
				break;
			}

			if(max_depth == 1) {
				game_board.bitBoardMoveGenerator(moveArray[0], stress);
				BitMove mv;
				uint8_t color;
				if(game_board.currentState.whiteMove) {
					color = WHITE;
				} else {
					color = BLACK;
				}
				int count = 0;
				for(unsigned int i = 0; i < moveArray[0].moveArray.size(); ++i) {
					game_board.move(moveArray[0].moveArray[i]);
					if(!game_board.inCheck(color)) {
						++count;
						mv = moveArray[0].moveArray[i];
					}
					
					game_board.goBack();
				}
				
				if(count == 1) {
					bestMove = mv;
					break;
				}
			}
		}

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true);
		
		if(stopped) {
			break;
		}

		hasBestMove = true;
		++max_depth;
	}*/

	BitBoard parallel_board[option.threads_count];

	for(int i = 0; i < option.threads_count; ++i) {
		parallel_board[i] = game_board;
	}

	moveArray.resize(option.threads_count);
	for(int i = 0; i < moveArray.size(); ++i) {
		moveArray[i].resize(200);
	}

	for(; timer.getTime() < time; ) {
		
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		if(tournamentTimeControll) {
			if(timer.getTime() * 2 >= time) {
				break;
			}

			if(max_depth == 1) {
				game_board.bitBoardMoveGenerator(moveArray[0][0], stress);
				BitMove mv;
				uint8_t color;
				if(game_board.currentState.whiteMove) {
					color = WHITE;
				} else {
					color = BLACK;
				}
				
			}
		}
		
		//negamax(parallel_board[0], -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true);

		std::vector<Killers> killers(option.threads_count);

		std::thread thread[option.threads_count];
		bool mainThread = true;
		for(int i = 0; i < option.threads_count; ++i) {
			thread[i] = std::thread(&Game::negamax, this, std::ref(parallel_board[i]), std::ref(moveArray[i]), -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true, i, std::ref(killers[i]));
			mainThread = false;
			//negamax(parallel_board[i], moveArray[i], -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true);
			
		}

		for(int i = 0; i < option.threads_count; ++i) {
			thread[i].join();	
		}
		
		if(stopped) {
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
	if(game_board.currentState.whiteMove) {
		tm = wtime;
		inc = winc;
	} else {
		tm = btime;
		inc = binc;
	}

	double k = 40;

	if(movestogoEnable) {
		k = movestogo;
		goFixedTime(tm / k + inc / 2, true);
	} else {
		goFixedTime(tm / k + inc / 2 - 100, true);
	}
}

bool Game::move(std::string mv) {
	MoveArray moves;
	game_board.bitBoardMoveGenerator(moves, stress);
	for(unsigned int i = 0; i < moves.count; ++i) {
		if(moves.moveArray[i].getMoveString() == mv) {
			game_board.move(moves.moveArray[i]);
			uint8_t color;

			if(game_board.currentState.whiteMove) {
				color = BLACK;
			} else {
				color = WHITE;
			}

			if(game_board.inCheck(color)) {
				game_board.goBack();
				continue;
			}

			++hash_decrement;
			++hashAge;
			return true;
		}
	}

	return false;
}
