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
   int64_t lowerbound = -WHITE_WIN;
   int64_t upperbound = WHITE_WIN;
   int64_t g = f;
   while(lowerbound < upperbound) {
      int64_t beta = std::max(g, lowerbound + 1);
      g = negamax(game_board, beta - 1, beta, depth, 0, FIXED_DEPTH, false, true, true);
 
	  if(g < beta) {
		  upperbound = g;
	  } else {
		  lowerbound = g;
	  }
   }
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

	start_timer = clock();
	hasBestMove = false;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	hasBestMove = true;
	bestScore = 0;
	int64_t f = 0;
	
	//int64_t current_alpha = -WHITE_WIN, current_beta = WHITE_WIN, win_size = 50;

	for(; max_depth <= max_depth_global; ++max_depth) {
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true, true);

		/*if(max_depth > 1) {
			current_alpha = bestScore - 30;
			current_beta = bestScore + 30;
		}*/

		//negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, true);
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

	for(; timer.getTime() < time; ) {
		
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

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, true, true);
		
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

void Game::mctsEval(int gamesCount) {
	int whiteWin = 0;
	int blackWin = 0;
	int draw = 0;
	for(int i = 0; i < gamesCount; ++i) {
		std::cout << i << "/" << gamesCount << std::endl;
		int res = playGame();

		if(res == 1) {
			++whiteWin;
		} else if(res == -1) {
			++blackWin;
		} else {
			++draw;
		}
	}

	std::cout << (whiteWin + draw * 0.5) / (whiteWin + blackWin + draw) * 100 << "%" << std::endl;
	std::cout << "W: " << whiteWin << "; B: " << blackWin << "; D: " << draw << std::endl;

}

int Game::playGame() {
	std::string fen = game_board.getFen();

	std::vector<BitMove>possibleMoves;

	for(int i = 0; i < 200; ++i) {
		possibleMoves.clear();
		if(game_board.currentState.ruleNumber >= 100) {
			game_board.setFen(fen);
			return 0;
		}
		size_t count = 0;
		game_board.bitBoardMoveGenerator(moveArray[0], count);


		for(int i = 0; i < moveArray[0].count; ++i) {
			game_board.move(moveArray[0].moveArray[i]);

			/*if(game_board.currentState.whiteMove) {
				if(!game_board.inCheck(BLACK)) {
					possibleMoves.push_back(moveArray[0].moveArray[i]);
				}
			} else {
				if(!game_board.inCheck(WHITE)) {
					possibleMoves.push_back(moveArray[0].moveArray[i]);
				}
			}*/

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

			possibleMoves.push_back(moveArray[0].moveArray[i]);
			
			game_board.goBack();
		}

		if(possibleMoves.size() == 0) {
			if(game_board.currentState.whiteMove) {
				if(game_board.inCheck(WHITE)) {
					game_board.setFen(fen);
					return -1;
				} else {
					game_board.setFen(fen);
					return 0;
				}
			} else {
				if(game_board.inCheck(BLACK)) {
					game_board.setFen(fen);
					return 1;
				} else {
					game_board.setFen(fen);
					return 0;
				}
			}
		}

		int nextMove = rand() % possibleMoves.size();

		game_board.move(possibleMoves[nextMove]);
	}

	game_board.setFen(fen);
	return 0;
}