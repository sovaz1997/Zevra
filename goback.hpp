#ifndef GOBACK_HPP
#define GOBACK_HPP

#include <vector>
#include <cstdint>
#include "constants.hpp"
#include "figurecell.hpp"

class GoBack {
public:
	GoBack();
 	uint64_t figures[7];
	uint64_t castlingMap, hash, recapture_position, piece_bit_mask[2];
	uint8_t passant_x, passant_y;
	uint16_t moveNumber, ruleNumber;
	bool color, passant_enable;
	bool attacked;
	double margin;
	bool hash_enable;
	bool whitePassantMade, blackPassantMade;
};

#endif
