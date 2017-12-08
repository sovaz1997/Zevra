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
	//std::vector<std::vector<uint8_t>>board_all_figures;
	uint64_t white_bit_mask, black_bit_mask, castlingMap, hash, recapture_position;
	uint8_t passant_x, passant_y;
	uint16_t moveNumber, ruleNumber;
	bool whiteMove, passant_enable;
	//int64_t evalute;
	bool attacked;
	double margin;
	bool hash_enable;
	bool whitePassantMade, blackPassantMade;
};

#endif
