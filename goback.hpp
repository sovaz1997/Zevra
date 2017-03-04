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

#ifndef GOBACK_HPP
#define GOBACK_HPP

#include <cstdint>
#include "constants.hpp"

class GoBack {
public:
	GoBack();
 	uint64_t figures[7];
	uint64_t white_bit_mask, black_bit_mask, castlingMap, hash;
	uint8_t moveNumber, ruleNumber, passant_x, passant_y;
	bool whiteMove, passant_enable;
	int64_t evalute;
	bool attacked;
	double margin;
	bool hash_enable;
	bool whitePassantMade, blackPassantMade;
};

#endif
