#include "game.hpp"

void Game::printScore(double score, int score_type) {
	std::cout << "score ";
	if(score > BLACK_WIN + 10000 && score < WHITE_WIN - 10000) {
		std::cout << "cp " << (int) (score / PAWN_EV.mg * 100);
	} else if(score < 0) {
		std::cout << "mate " <<  (int)((-WHITE_WIN - score) / 2);
	} else {
		std::cout << "mate " << (int)((WHITE_WIN - score + 1) / 2);
	}

	if(score_type == LOWERBOUND) {
		std::cout << " lowerbound";
	} else if(score_type == UPPERBOUND) {
		std::cout << " upperbound";
	}
}