#include "bitmove.hpp"

BitMove::BitMove() : category(0, 0), isAttack(false), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	category(0, 0), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(false), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	category(0, 0), attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt) :
	category(0, 0), attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true), replaced(false), passant(psnt), fromHash(false) {}

std::string BitMove::getMoveString() {
	std::string res;
	res.push_back(fromX + 'a');
	res.push_back(fromY + '1');
	res.push_back(toX + 'a');
	res.push_back(toY + '1');

	if(replaced) {
		if((replacedFigure & TYPE_SAVE) == KNIGHT) {
			res.push_back('n');
		} else if((replacedFigure & TYPE_SAVE) == BISHOP) {
			res.push_back('b');
		} else if((replacedFigure & TYPE_SAVE) == ROOK) {
			res.push_back('r');
		} else if((replacedFigure & TYPE_SAVE) == QUEEN) {
			res.push_back('q');
		}
	}

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
	//return category < mv.category;
}

bool BitMove::operator<(BitMove& mv) {
	return getAttackPrice() > mv.getAttackPrice();
	//return category > mv.category;
}

void BitMove::setReplaced(uint8_t figure) {
	replacedFigure = figure;
	replaced = true;
}

void BitMove::setCategory(int cat, double val) {
	category = Category(cat, val);
}

bool BitMove::quality() {
	return (fromY >= 0 && fromY <= 7 &&
					fromX >= 0 && fromX <= 7 &&
					toY >= 0 && toY <= 7 &&
					toX >= 0 && toY <= 7);
}
