#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include "constants.hpp"
#include "category.hpp"

class BitMove {
private:
	//Category category;
public:
	uint8_t attackedFigure, movedFigure, fromY, fromX, toY, toX;
	uint8_t replacedFigure;
	bool isAttack, replaced, passant, fromHash;

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
	void setCategory(int cat, double val);
	bool quality();
};

/*
class BitMove {
private:
	//Category category;
public:
	uint32_t data;

	void setAttackedFigure(uint8_t val);
	void setReplacedFigure(uint8_t val);
	void setReplacedFigure(uint8_t val);

	uint8_t attackedFigure, movedFigure, fromY, fromX, toY, toX;
	uint8_t replacedFigure;
	bool isAttack, replaced, passant, fromHash;

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
	void setCategory(int cat, double val);
	bool quality();
};*/

/*
Новый BitMove:

4 bits * 3 - figures;
6 bit - fromY/fromX;
6 bit-toY/toX
4 bit - other flags
All: uint32_t (28 bit)
*/

#endif
