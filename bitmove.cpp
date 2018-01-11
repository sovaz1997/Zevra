#include "bitmove.hpp"

BitMove::BitMove() : isAttack(false), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(false), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt) :
	attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true), replaced(false), passant(psnt), fromHash(false) {}

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

double BitMove::getAttackPrice() const {
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
	return (mv.fromY == fromY && mv.fromX == fromX && mv.toY == toY && mv.toX == toX && mv.movedFigure == movedFigure && (!mv.replaced || mv.replacedFigure == replacedFigure));
}

bool BitMove::operator>(const BitMove& mv) const {
	if(historyCompare) {
		return history_weight < mv.history_weight;
	}

	return getAttackPrice() < mv.getAttackPrice();
}

bool BitMove::operator<(const BitMove& mv) const {
	if(historyCompare) {
		return history_weight > mv.history_weight;
	}

	return getAttackPrice() > mv.getAttackPrice();
}

void BitMove::setReplaced(uint8_t figure) {
	replacedFigure = figure;
	replaced = true;
}

bool BitMove::quality() {
	return (fromY <= 7 && fromX <= 7 && toY <= 7 && toY <= 7);
}

void BitMove::printInfo() {
	std::cout << "move: " << getMoveString() << std::endl;
	std::cout << "attackedFigure: " << (int)attackedFigure << std::endl;
	std::cout << "movedFigure: " << (int)movedFigure << std::endl;
	std::cout << "replacedFigure: " << (int)replacedFigure << std::endl;
	std::cout << "isAttack: " << (int)isAttack << std::endl;
	std::cout << "replaced: " << (int)replaced << std::endl;
	std::cout << "passant: " << (int)passant << std::endl;
	std::cout << "fromHash: " << (int)fromHash << std::endl;
	std::cout << "history_weight: " << (int)history_weight << std::endl;
	std::cout << "historyCompare: " << (int)historyCompare << std::endl;
}