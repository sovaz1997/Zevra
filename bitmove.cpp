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

int BitMove::getAttackPrice() const {
	if(!isAttack) {
		return -INFINITY;
	}

	return attackedPrice[attackedFigure & TYPE_SAVE] * 100 - attackedPrice[movedFigure & TYPE_SAVE];
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