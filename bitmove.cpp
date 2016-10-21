#include "bitmove.hpp"

BitMove::BitMove() {

}


BitMove::BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx) {}

std::string BitMove::getMoveString() {
	std::string res;
	res.push_back(fromX + 'a');
	res.push_back(fromY + '1');
	res.push_back(toX + 'a');
	res.push_back(toY + '1');
	return res;
}