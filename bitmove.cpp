#include "bitmove.hpp"

BitMove::BitMove() {

}


BitMove::BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true) {}

std::string BitMove::getMoveString() {
	std::string res;
	res.push_back(fromX + 'a');
	res.push_back(fromY + '1');
	res.push_back(toX + 'a');
	res.push_back(toY + '1');
	return res;
}

double BitMove::getAttackPrice() {
	if(!isAttack) {
		return -INFINITY;
	}
	double res = 0;

	if((attackedFigure & TYPE_SAVE) == QUEEN) {
		res = QUEEN_EV * 100;
	} else if((attackedFigure & TYPE_SAVE) == ROOK) {
		res = ROOK_EV * 100;
	} else if((attackedFigure & TYPE_SAVE) == BISHOP) {
		res = BISHOP_EV * 100;
	} else if((attackedFigure & TYPE_SAVE) == KNIGHT) {
		res = KNIGHT_EV * 100;
	} else if((attackedFigure & TYPE_SAVE) == PAWN) {
		res = PAWN_EV;
	}

	if((movedFigure & TYPE_SAVE) == QUEEN) {
		return res - QUEEN_EV;
	} else if((movedFigure & TYPE_SAVE) == ROOK) {
		return res - ROOK_EV;
	} else if((movedFigure & TYPE_SAVE) == BISHOP) {
		return res - BISHOP_EV;
	} else if((movedFigure & TYPE_SAVE) == KNIGHT) {
		return res - KNIGHT_EV;
	} else if((movedFigure & TYPE_SAVE) == PAWN) {
		return res - PAWN_EV;
	}

	return res;
}

bool BitMove::equal(BitMove& mv) {
	return (mv.fromY == fromY && mv.fromX == fromX && mv.toY == toY && mv.toX == toX && mv.movedFigure == movedFigure);
}

bool BitMove::operator>(BitMove& mv) {
	return getAttackPrice() < mv.getAttackPrice();
}

bool BitMove::operator<(BitMove& mv) {
	return getAttackPrice() > mv.getAttackPrice();
}
