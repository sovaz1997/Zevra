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

void Game::go() {
		clearCash();

		variant.clear();
		variant.resize(max_depth);
		double win;
		std::vector<uint64_t>hash;

		pv_best.clear();
		std::vector<Move>pv;
		whiteUp = BLACK_WIN;
		blackUp = WHITE_WIN;

		bool basis = false;
		movesCounter = 0;
		double start_timer, end_timer;
		//boardHash.clear();

		for(int i = max_depth; i <= max_depth; ++i) {
			//flattenHistory();
			cleanWhiteHistory();
			cleanBlackHistory();
			double movesCounterTmp = movesCounter;
			if(i == max_depth) {
				basis = true;
			}

			if(game_board.isWhiteMove()) {
				start_timer = clock();
				win = minimax_white(game_board, -INFINITY, INFINITY, 0, i, 0, gameHash, basis, pv, true);
				end_timer = clock();
			} else {
				start_timer = clock();
				win = minimax_black(game_board, -INFINITY, INFINITY, 0, i, 0, gameHash, basis, pv, true);
				end_timer = clock();
			}

			std::cout << "info nps " << (int)((movesCounter - movesCounterTmp) / ((end_timer - start_timer) / CLK_TCK)) << "\n";
		}

	for(int i = 0; i < pv_best.size(); ++i) {
		pv_best[i].print();
		std::cout << " ";
	}

	std::cout << "\n";
}

bool Game::move(std::string mv) {
	bool tmp_shah = false;
	std::vector<Move>moves = generatePositionMoves(game_board, tmp_shah, true, 0);

	for(unsigned int i = 0; i < moves.size(); ++i) {
		if(moves[i].getMoveString() == mv) {
			Board tmp = game_board;
			game_board.move(moves[i]);
			bool shah = false;
			std::vector<Move>moves = generatePositionMoves(game_board, shah, true, 0);

			if(shah) {
				game_board = tmp;
				continue;
			}

			gameHash.push_back(getHash(game_board));

			return true;
		}
	}

	return false;
}
