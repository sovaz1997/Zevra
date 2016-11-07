#include "game.hpp"

void Game::printScore(double score) {
	std::cout << "score ";
	if(score > BLACK_WIN + 10000 && score < WHITE_WIN - 10000) {
		std::cout << "cp " << (int) (score / PAWN_EV * 100);
	} else if(score < 0) {
		std::cout << "score mate " <<  (int)(-abs(score - BLACK_WIN) / 2 - 1);
	} else {
		std::cout << "mate " <<  (int)(abs(score - WHITE_WIN) / 2 + 1);
	}
}
