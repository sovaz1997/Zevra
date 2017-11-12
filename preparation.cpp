#include "game.hpp"

void Game::initEngine() {
	/*whiteKiller.resize(100);
	blackKiller.resize(100);
	whiteSecondKiller.resize(100);
	blackSecondKiller.resize(100);
	whiteMateKiller.resize(100);
	blackMateKiller.resize(100);*/

    setHashSize(option.hash_size);

	for(int i = 0; i < BOARD_SIZE; ++i) {
		for(int j = 0; j < BOARD_SIZE; ++j) {
			for(int k = 0; k < BOARD_SIZE; ++k) {
				for(int m = 0; m < BOARD_SIZE; ++m) {
					evalute_cells_size[i][j][k][m]  = sqrt(pow(i - k, 2) + pow(j - m, 2)) + 5;
				}
			}
		}
	}

	cleanWhiteHistory();
	cleanBlackHistory();
	clearCash();

	hash_filled = 0;
	max_hash_filled = boardHash.size();

	initializeArrays();
}

void Game::initializeArrays() {
    /*figureToIndex[EMPTY] = 0;
    figureToIndex[KING | WHITE] = 1;
    figureToIndex[QUEEN | WHITE] = 2;
    figureToIndex[ROOK | WHITE] = 3;
    figureToIndex[BISHOP | WHITE] = 4;
    figureToIndex[KNIGHT | WHITE] = 5;
    figureToIndex[PAWN | WHITE] = 6;
    figureToIndex[KING | BLACK] = 7;
    figureToIndex[QUEEN | BLACK] = 8;
    figureToIndex[ROOK | BLACK] = 9;
    figureToIndex[BISHOP | BLACK] = 10;
    figureToIndex[KNIGHT | BLACK] = 11;

    indexToFigure[0] = EMPTY;
    indexToFigure[1] = KING | WHITE;
    indexToFigure[2] = QUEEN | WHITE;
    indexToFigure[3] = ROOK | WHITE;
    indexToFigure[4] = BISHOP | WHITE;
    indexToFigure[5] = KNIGHT | WHITE;
    indexToFigure[6] = PAWN | WHITE;
    indexToFigure[7] = KING | BLACK;
    indexToFigure[8] = QUEEN | BLACK;
    indexToFigure[9] = ROOK | BLACK;
    indexToFigure[10] = BISHOP | BLACK;
    indexToFigure[11] = KNIGHT | BLACK;*/
}