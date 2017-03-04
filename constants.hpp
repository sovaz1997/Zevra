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

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <cwchar>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

bool is_input_available();

const uint8_t PAWN = 1;    //00000001
const uint8_t KNIGHT = 2;  //00000010
const uint8_t BISHOP = 3;  //00000011
const uint8_t ROOK = 4;    //00000100
const uint8_t QUEEN = 5;   //00000101
const uint8_t KING = 6;    //00000110

const uint8_t WHITE = 8;   //00001000
const uint8_t BLACK = 16;  //00010000

const uint8_t COLOR_SAVE = 24;  //00011000
const uint8_t TYPE_SAVE = 7; //00000111

const int BOARD_SIZE = 8;

const uint8_t STANDARD_MV = 0;
const uint8_t WS_CASTLING_MV = 1;
const uint8_t WL_CASTLING_MV = 2;
const uint8_t BS_CASTLING_MV = 3;
const uint8_t BL_CASTLING_MV = 4;
const uint8_t PASSANT_MV = 5;
const uint8_t TRANSFORMATION_MV = 6;

const uint64_t reverse_color_const = 54387554323743313;

const double PAWN_EV = 100;
const double KNIGHT_EV = 320;
const double BISHOP_EV = 330;
const double ROOK_EV = 500;
const double QUEEN_EV = 900;

const double ENDGAME_PAWN_EV = 150;
const double ENDGAME_KNIGHT_EV = 340;
const double ENDGAME_BISHOP_EV = 350;

//move types
const int FIXED_DEPTH = 0;
const int FIXED_TIME = 1;
const int TOURNAMENT = 2;

const int hash_width = 22;

const int REAL_EV = 0;
const int BETA_CUT_EV = 1;
const int ALPHA_CUT_EV = 2;


const int EMPTY = 0;
const int EXACT = 1;
const int ALPHA = 2;
const int BETA = 3;

const int MIN_DEPTH = 2;

const uint64_t hash_cutter = std::pow(2, hash_width) - 1;

const double ALL_MATERIAL = 16 * PAWN_EV
+	4 * KNIGHT_EV
+	4 * BISHOP_EV
+	4 * ROOK_EV
+	2 * QUEEN_EV;
/*
const double pawnMatr[BOARD_SIZE][BOARD_SIZE] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{50, 50, 50, 50, 50, 50, 50, 50},
{10, 10, 20, 30, 30, 20, 10, 10},
{5, 5, 10, 25, 25, 10, 5, 5},
{0, 0, 0, 20, 20, 0, 0, 0},
{5, -5, -10, 0, 0, -10, -5, 5},
{5, 10, 10, -20, -20, 10, 10, 5},
{0, 0, 0, 0, 0, 0, 0, 0}
};

const double endGamePawnMatr[BOARD_SIZE][BOARD_SIZE] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{80, 80, 80, 80, 80, 80, 80, 80},
{40, 40, 40, 40, 40, 40, 40, 40},
{25, 25, 25, 25, 25, 25, 25, 25},
{10, 10, 10, 10, 10, 10, 10, 10},
{-5, -5, -5, -5, -5, -5, -5, -5},
{-10, -10, -10, -10, -10, -10, -10, -10},
{0, 0, 0, 0, 0, 0, 0, 0}
};*/
const double pawnMatr[BOARD_SIZE][BOARD_SIZE] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{5, 10, 15, 20, 20, 15, 10, 5},
{4, 8, 12, 16, 16, 12, 8, 4},
{3, 6, 9, 12, 12, 9, 6, 3},
{2, 4, 6, 8, 8, 6, 4, 2},
{1, 2, 3, -10, -10, 3, 2, 1},
{0, 0, 0, -40, -40, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

const double knightMatr[BOARD_SIZE][BOARD_SIZE] {
{-50, -40, -30, -30, -30, -30, -40, -50},
{-40, -20, 0, 0, 0, 0, -20, -40},
{-30, 0, 10, 15, 15, 10, 0, -30},
{-30, 5, 15, 20, 20, 15, 5, -30},
{-30, 0, 15, 20, 20, 15, 5, -30},
{-30, 5, 10, 15, 15, 10, 0, -30},
{-40, -20, 0, 5, 5, 0, -20, -40},
{-50, -40, -30, -30, -30, -30, -40, -50}
};

const double bishopMatr[BOARD_SIZE][BOARD_SIZE] {
{-20, -10, -10, -10, -10, -10, -10, -20},
{-10, 0, 0, 0, 0, 0, 0, -10},
{-10, 0, 5, 10, 10, 5, 0, -10},
{-10, 5, 5, 10, 10, 5, 5, -10},
{-10, 0, 10, 10, 10, 10, 0, -10},
{-10, 10, 10, 10, 10, 10, 10, -10},
{-10, 5, 0, 0, 0, 0, 5, -10},
{-20, -10, -10, -10, -10, -10, -10, -20}
};

const double rookMatr[BOARD_SIZE][BOARD_SIZE] {
{0, 0, 0, 0, 0, 0, 0, 0},
{5, 10, 10, 10, 10, 10, 10, 5},
{-5, 0, 0, 0, 0, 0, 0, -5},
{-5, 0, 0, 0, 0, 0, 0, -5},
{-5, 0, 0, 0, 0, 0, 0, -5},
{-5, 0, 0, 0, 0, 0, 0, -5},
{-5, 0, 0, 0, 0, 0, 0, -5},
{0, 0, 0, 5, 5, 0, 0, 0}
};

const double queenMatr[BOARD_SIZE][BOARD_SIZE] {
{-20, -10, -10, -5, -5, -10, -10, -20},
{-10, 0, 0, 0, 0, 0, 0, -10},
{-10, 0, 5, 5, 5, 5, 0, -10},
{-5, 0, 5, 5, 5, 5, 0, -5},
{0, 0, 5, 5, 5, 5, 0, -5},
{-10, 0, 5, 0, 0, 0, 0, -10},
{-20, -10, -10, -5, -5, -10, -10, -20}
};

const double kingDebuteMatr[BOARD_SIZE][BOARD_SIZE] {
{-30, -40, -40, -50, -50, -40, -40, -30},
{-30, -40, -40, -50, -50, -40, -40, -30},
{-30, -40, -40, -50, -50, -40, -40, -30},
{-30, -40, -40, -50, -50, -40, -40, -30},
{-20, -30, -30, -40, -40, -30, -30, -20},
{-10, -20, -20, -20, -20, -20, -20, -10},
{20, 20, 0, 0, 0, 0, 20, 20},
{20, 30, 10, 0, 0, 10, 30, 20}
};

const double kingEndGameMatr[BOARD_SIZE][BOARD_SIZE] {
{-50, -40, -30, -20, -20, -30, -40, -50},
{-30, -20, -10, 0, 0, -10, -20, -30},
{-30, -10, 20, 30, 30, 20, -10, -30},
{-30, -10, 30, 40, 40, 30, -10, -30},
{-30, -10, 30, 40, 40, 30, -10, -30},
{-30, -10, 20, 30, 30, 20, -10, -30},
{-50, -30, -30, -30, -30, -30, -30, -50}
};

const double PASSED_PAWN_BONUS = 50;
const double DUAL_PAWN_BONUS = -15;
const double ISOLATED_PAWN_BONUS = -15;
const double passed_pawn_line[BOARD_SIZE] = {0, 90, 60, 50, 40, 30, 20, 0};

const double attack_bonus[7] = {0, 1, 3, 6, 10, 14, 20};
const double king_security[12] = {10, 9, 8, 7, 5, 3, 2, 1, 0, -1, -2, -3};

#endif
