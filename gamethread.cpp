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

void Game::goFixedDepth() {
	++hashAge;
	
	if(option.UCI_AnalyseMode || abs(bestScore) >= (WHITE_WIN - 100)) {
		clearCash();
	}
	
	stopped = false;
	std::vector<uint64_t>hash;

	nodesCounter = 0;
	int max_depth_global = max_depth;
	max_depth = 1;

	start_timer = clock();
	hasBestMove = false;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	hasBestMove = true;
	bestScore = 0;
	int f = 0;

	for(; max_depth <= max_depth_global; ++max_depth) {
		int window = 30;
		
		flattenHistory();

		int a = f - window, b = f + window;
		std::cout << a << " " << b << "\n";
		a = std::max(-WHITE_WIN, a);
		b = std::min(WHITE_WIN, b);
		while(true) {
			f = negamax(game_board, a, b, max_depth, 0, FIXED_DEPTH, false, true);
			std::cout << "Score: " << f << "\n";

			if (f > a && f < b) {
				break;
			}

			if(f <= a) {
				b = (a + b) / 2;
				a = std::max(-WHITE_WIN, a - window);
			}

			if(f >= b) {
				b = std::min(WHITE_WIN, b + window);
			}

				window = window + window / 2;

			if(stopped) {
				break;
			}
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

void Game::goFixedTime(int64_t tm, bool tournamentTimeControll) {
	tm -= option.moveOverhead;
	tm = std::max(tm, (int64_t)10);

	++hashAge;
	stopped = false;

	time = tm;
	timer.start();

	if(option.UCI_AnalyseMode || abs(bestScore) >= (WHITE_WIN - 100)) {
		clearCash();
	}

	std::vector<uint64_t>hash;

	nodesCounter = 0;

	start_timer = clock();
	hasBestMove = false;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	bestScore = 0;
	hasBestMove = true;
	max_depth = 1;

	std::vector<BitMove> bestPV;

	int f = 0;

	for(; timer.getTime() < time; ) {
		int window = 30;
		int a = -window, b = window;
		a = std::max(-WHITE_WIN, a);
		b = std::min(WHITE_WIN, b);

		flattenHistory();

		if(tournamentTimeControll) {
			if(timer.getTime() * 2 >= time) {
				break;
			}

			if(max_depth == 1) {
				game_board.bitBoardMoveGenerator(moveArray[0]);
				BitMove mv;
				uint8_t color;
				if(game_board.currentState.color) {
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
		
		flattenHistory();

		while(true) {
			f = negamax(game_board, a, b, max_depth, 0, FIXED_TIME, false, true);

			if (f > a && f < b) {
				break;
			}

			if(f <= a) {
				b = (a + b) / 2;
				a = std::max(-WHITE_WIN, a - window);
			}

			if(f >= b) {
				b = std::min(WHITE_WIN, b + window);
			}

			window = window + window / 2;
			
			if(stopped) {
				break;
			}
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
	if(game_board.currentState.color) {
		tm = wtime;
		inc = winc;
	} else {
		tm = btime;
		inc = binc;
	}

	double k;

	int figuresNumber = game_board.popcount64(game_board.currentState.piece_bit_mask[whiteSide] | game_board.currentState.piece_bit_mask[!whiteSide]);

	if(movestogoEnable) {
		k = movestogo;
		goFixedTime(tm / (k + 1) + inc / 2, true);
	} else {
		k = 40 - (32 - figuresNumber);
		goFixedTime(tm / k + inc / 2 - 100, true);
	}
}

bool Game::move(std::string mv) {
	MoveArray moves;
	game_board.bitBoardMoveGenerator(moves);
	for(unsigned int i = 0; i < moves.count; ++i) {
		if(moves.moveArray[i].getMoveString() == mv) {
			game_board.move(moves.moveArray[i]);
			uint8_t color;

			if(game_board.currentState.color) {
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
