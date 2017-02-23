#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include "constants.hpp"
#include "category.hpp"

class BitMove {
public:
	uint8_t attackedFigure, movedFigure, fromY, fromX, toY, toX;
	uint8_t replacedFigure;
	bool isAttack, replaced, passant, fromHash;

	//uint32_t data = 0;

	/*uint8_t getAttackedFigure();
	uint8_t getMovedFigure();
	uint8_t getFromY();
	uint8_t getFromX();
	uint8_t getToY();
	uint8_t getToX();
	uint8_t getReplacedFigure();
	bool getIsAttack();
	bool getReplaced();
	bool getPassant();
	bool getFromHash();

	void setAttackedFigure(uint8_t value);
	void setMovedFigure(uint8_t value);
	void setFromY(uint8_t value);
	void setFromX(uint8_t value);
	void setToY(uint8_t value);
	void setToX(uint8_t value);
	void setReplacedFigure(uint8_t value);
	void setIsAttack(bool value);
	void setReplaced(bool value);
	void setPassant(bool value);
	void setFromHash(bool value);*/



	BitMove();
	BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt);

	bool operator>(const BitMove& mv) const;
	bool operator<(const BitMove& mv) const;
	std::string getMoveString();
	double getAttackPrice() const;
	bool equal(BitMove& mv);
	void setReplaced(uint8_t figure);
	bool quality();
};

#endif
