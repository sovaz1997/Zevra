#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <string>
#include <stdint.h>

class BitMove {
public:
	uint8_t fromY, fromX, toY, toX;
	uint8_t movedFigure;

  BitMove();
  BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	std::string getMoveString();
};

#endif
