#include "game.hpp"
#include "bitboard.hpp"

int main() {
	BitBoard board;
	board.setFen("r1bq1rk1/pppp1ppp/2n2n2/1B2p3/1b2P3/2N2N2/PPPPQPPP/R1B1K2R w KQ - 8 6");
	std::cout << board.getFen();

	Game game;
	game.startGame();
}
