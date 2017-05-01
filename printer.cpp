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

std::string Game::getScore(double score) {
	std::string result;
	result += "score ";
	if(score > BLACK_WIN + 10000 && score < WHITE_WIN - 10000) {
		result += "cp ";
		result += std::to_string((int) (score / PAWN_EV * 100));
	} else if(score < 0) {
		result += "score mate ";
		result += std::to_string((int)(-abs(score - BLACK_WIN) / 2 - 1));
	} else {
		result += "mate ";
		result += std::to_string((int)(abs(score - WHITE_WIN) / 2 + 1));
	}

	return result;
}
