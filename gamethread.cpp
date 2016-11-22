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

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	std::vector<BitMove>pv;
	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;
	int max_depth_global = max_depth;
	max_depth = 1;

	start_timer = clock();
	hasBestMove = false;
	double depth;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	hasBestMove = true;
	bestScore = 0;

	for(; max_depth <= max_depth_global; ++max_depth) {
		pv.resize(0);
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_DEPTH, false, false, new PV());
		hasBestMove = true;

		if(abs(bestScore) >= (WHITE_WIN - 100)) {
			break;
		}

		depth = max_depth;
	}

	end_timer = clock();

	std::cout << "info depth " << depth << " ";
	printScore(bestScore);
	std::cout << " pv " << bestMove.getMoveString();
	std::cout << " nodes " << nodesCounter << " nps " << (int)(nodesCounter / ((end_timer - start_timer) / CLOCKS_PER_SEC)) <<
	" time " << (int)((end_timer - start_timer) / (CLOCKS_PER_SEC / 1000)) << "\n";
	if(hasBestMove) {
		std::cout << "bestmove " << bestMove.getMoveString() << "\n";
	}
}

void Game::goFixedTime(int tm) {
	if(tm >= 200) {
		tm -= 100;
	}

	time = tm;
	timer.start();

	clearCash();

	variant.clear();
	variant.resize(max_depth);
	std::vector<uint64_t>hash;

	std::vector<BitMove>pv;
	whiteUp = BLACK_WIN;
	blackUp = WHITE_WIN;

	nodesCounter = 0;

	start_timer = clock();
	hasBestMove = false;
	double depth;

	BitMove moveCritical = game_board.getRandomMove();
	bestMove = moveCritical;
	bestScore = 0;
	hasBestMove = true;

	for(max_depth = 1; timer.getTime() < time; ++max_depth) {
		pv.resize(0);
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;
		flattenHistory();

		negamax(game_board, -WHITE_WIN, WHITE_WIN, max_depth, 0, FIXED_TIME, false, false, new PV());

		if(abs(bestScore) >= (WHITE_WIN - 100)) {
			break;
		}

		hasBestMove = true;
		depth = max_depth;
	}

	end_timer = clock();

	std::cout << "info depth " << depth << " ";
	printScore(bestScore);
	std::cout << " pv " << bestMove.getMoveString();
	std::cout << " nodes " << nodesCounter << " nps " << (int)(nodesCounter / ((end_timer - start_timer) / CLOCKS_PER_SEC)) <<
	" time " << (int)((end_timer - start_timer) / (CLOCKS_PER_SEC / 1000)) << "\n";
	if(hasBestMove) {
		std::cout << "bestmove " << bestMove.getMoveString() << "\n";
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

			return true;
		}
	}

	return false;
}
