#include "move.hpp"

Move::Move() {}
Move::Move(uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx) {
	init(fy, fx, ty, tx, STANDARD_MV);
}

Move::Move(std::string mv) {
	uint8_t fx = mv[0] - 'a';
	uint8_t fy = 7 - (mv[1] - '1');
	uint8_t tx = mv[2] - 'a';
	uint8_t ty = 7 - (mv[3] - '1');

	init(fy, fx, ty, tx, STANDARD_MV);
}

void Move::go(Board* b) {
	if(moveType == STANDARD_MV || moveType == PASSANT_MV || moveType == TRANSFORMATION_MV) {
		b->setFigure(b->getFigure(fromY, fromX), toY, toX);
		b->setFigure(0, fromY, fromX);
	}

	if(moveType == PASSANT_MV) {
		b->setFigure(0, passant_y, passant_x);
	}

	if(moveType == TRANSFORMATION_MV) {
		b->setFigure(figureTransform, toY, toX);
	}

	if(moveType == WS_CASTLING_MV) {
		b->setFigure(0, 7, 7);
		b->setFigure(0, 7, 4);
		b->setFigure(KING | WHITE, 7, 6);
		b->setFigure(ROOK | WHITE, 7, 5);
	}

	if(moveType == WL_CASTLING_MV) {
		b->setFigure(0, 7, 0);
		b->setFigure(0, 7, 4);
		b->setFigure(KING | WHITE, 7, 2);
		b->setFigure(ROOK | WHITE, 7, 3);
	}

	if(moveType == BS_CASTLING_MV) {
		b->setFigure(0, 0, 7);
		b->setFigure(0, 0, 4);
		b->setFigure(KING | BLACK, 0, 6);
		b->setFigure(ROOK | BLACK, 0, 5);
	}

	if(moveType == BL_CASTLING_MV) {
		b->setFigure(0, 0, 0);
		b->setFigure(0, 0, 4);
		b->setFigure(KING | BLACK, 0, 2);
		b->setFigure(ROOK | BLACK, 0, 3);
	}
}

void Move::init(uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, uint8_t mt) {
	fromY = fy;
	fromX = fx;
	toY = ty;
	toX = tx;
	moveType = mt;
}

void Move::print() {
	std::cout << getMoveString();
}

std::string Move::getMoveString() {
	std::string res;

	res.push_back(fromX + 'a');
	res.push_back(7 - fromY + '1');
	res.push_back(toX + 'a');
	res.push_back(7 - toY + '1');


	if(moveType == TRANSFORMATION_MV) {
		if((figureTransform & TYPE_SAVE) == QUEEN) {
			res.push_back('q');
		} else if((figureTransform & TYPE_SAVE) == ROOK) {
			res.push_back('r');
		} else if((figureTransform & TYPE_SAVE) == BISHOP) {
			res.push_back('b');
		} else if((figureTransform & TYPE_SAVE) == KNIGHT) {
			res.push_back('n');
		}
	}

	return res;
}

double Move::getAttackPrice(Board & b) {
	if(b.getFigure(toY, toX) == 0) {
		return 0;
	}

	double res;

	if((b.getFigure(toY, toX) & TYPE_SAVE) == QUEEN) {
		res = QUEEN_EV;
	} else if((b.getFigure(toY, toX) & TYPE_SAVE) == ROOK) {
		res = ROOK_EV;
	} else if((b.getFigure(toY, toX) & TYPE_SAVE) == BISHOP) {
		res = BISHOP_EV;
	} else if((b.getFigure(toY, toX) & TYPE_SAVE) == KNIGHT) {
		res = KNIGHT_EV;
	} else if((b.getFigure(toY, toX) & TYPE_SAVE) == PAWN) {
		res = PAWN_EV;
	}

	if((b.getFigure(fromY, fromX) & TYPE_SAVE) == QUEEN) {
		return res - QUEEN_EV;
	} else if((b.getFigure(fromY, fromX) & TYPE_SAVE) == ROOK) {
		return res - ROOK_EV;
	} else if((b.getFigure(fromY, fromX) & TYPE_SAVE) == BISHOP) {
		return res - BISHOP_EV;
	} else if((b.getFigure(fromY, fromX) & TYPE_SAVE) == KNIGHT) {
		return res - KNIGHT_EV;
	} else if((b.getFigure(fromY, fromX) & TYPE_SAVE) == PAWN) {
		return res - PAWN_EV;
	}

	return 0;
}

bool Move::equal(Move & mv) {
	return fromX == mv.fromX && fromY == mv.fromY && toX == mv.toX && toY == mv.toY;
}

bool Move::quality() {
	return bit_quality(fromY) && bit_quality(fromX) && bit_quality(toY) && bit_quality(toX);
}

bool Move::bit_quality(int bit) {
	return bit >= 0 && bit < 8;
}

bool Move::operator>(Move& mv) {
	return this->simple_eval > mv.simple_eval;
}
bool Move::operator<(Move& mv) {
	return this->simple_eval < mv.simple_eval;
}
