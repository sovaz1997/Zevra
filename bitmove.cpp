/*
  Zevra, a UCI chess playing engine
  Copyright (C) 2016-2017 Oleg Smirnov (author)
  Zevra is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
  Zevra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "bitmove.hpp"

BitMove::BitMove() : /*category(0, 0),*/ isAttack(false), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	/*category(0, 0),*/ movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(false), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) :
	/*category(0, 0),*/ attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true), replaced(false), passant(false), fromHash(false) {}

BitMove::BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt) :
	/*category(0, 0),*/ attackedFigure(afig), movedFigure(fig), fromY(fy), fromX(fx), toY(ty), toX(tx), isAttack(true), replaced(false), passant(psnt), fromHash(false) {}

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
	return (mv.fromY == fromY && mv.fromX == fromX && mv.toY == toY && mv.toX == toX && mv.movedFigure == movedFigure);
}

bool BitMove::operator>(const BitMove& mv) const {
	return getAttackPrice() < mv.getAttackPrice();
	//return category < mv.category;
}

bool BitMove::operator<(const BitMove& mv) const {
	return getAttackPrice() > mv.getAttackPrice();
	//return category > mv.category;
}

void BitMove::setReplaced(uint8_t figure) {
	replacedFigure = figure;
	replaced = true;
}

bool BitMove::quality() {
	return (fromY >= 0 && fromY <= 7 &&
					fromX >= 0 && fromX <= 7 &&
					toY >= 0 && toY <= 7 &&
					toX >= 0 && toY <= 7);
}
/*
uint8_t BitMove::getAttackedFigure() {

}

uint8_t BitMove::getMovedFigure() {

}

uint8_t BitMove::getFromY() {

}

uint8_t BitMove::getFromX() {

}

uint8_t BitMove::getToY() {

}

uint8_t BitMove::getToX() {

}

uint8_t BitMove::getReplacedFigure() {

}

uint8_t BitMove::getIsAttack() {

}

uint8_t BitMove::getReplaced() {

}

uint8_t BitMove::getPassant() {

}

uint8_t BitMove::getFromHash() {

}

void BitMove::setAttackedFigure(uint8_t value) {
	
}

void BitMove::setMovedFigure(uint8_t value) {

}

void BitMove::setFromY(uint8_t value) {

}

void BitMove::setFromX(uint8_t value) {

}

void BitMove::setToY(uint8_t value) {

}

void BitMove::setToX(uint8_t value) {

}

void BitMove::setReplacedFigure(uint8_t value) {

}

void BitMove::setIsAttack(bool value) {

}

void BitMove::setReplaced(bool value) {

}

void BitMove::setPassant(bool value) {

}

void BitMove::setFromHash(bool value) {

}*/