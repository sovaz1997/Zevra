#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <vector>
#include <cstdint>
#include <cwchar>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "score.hpp"

using S = Score;

bool is_input_available();

//Название движка
const std::string engine_name = "Zevra 20180621";
const std::string engine_author = "Oleg Smirnov";


// Битовое обозначение фигур

const uint8_t PAWN = 1;    //00000001
const uint8_t KNIGHT = 2;  //00000010
const uint8_t BISHOP = 3;  //00000011
const uint8_t ROOK = 4;    //00000100
const uint8_t QUEEN = 5;   //00000101
const uint8_t KING = 6;    //00000110

// Цвет фигур
const uint8_t WHITE = 8;   //00001000
const uint8_t BLACK = 16;  //00010000

// Маски фигур (для разделения фигуры и цвета)
const uint8_t COLOR_SAVE = 24;  //00011000
const uint8_t TYPE_SAVE = 7; //00000111


const int BOARD_SIZE = 8;
const bool whiteSide = true;

// Типы ходов
const uint8_t STANDARD_MV = 0;
const uint8_t WS_CASTLING_MV = 1;
const uint8_t WL_CASTLING_MV = 2;
const uint8_t BS_CASTLING_MV = 3;
const uint8_t BL_CASTLING_MV = 4;
const uint8_t PASSANT_MV = 5;
const uint8_t TRANSFORMATION_MV = 6;

const uint64_t reverse_color_const = 54387554323743313;

// Веса фигур Mg и Eg
const Score PAWN_EV = S(100, 140);
const Score KNIGHT_EV = S(400, 420);
const Score BISHOP_EV = S(430, 460);
const Score ROOK_EV = S(650, 700);
const Score QUEEN_EV = S(1100, 1200);

//Типы ходов
const int FIXED_DEPTH = 0; //фикс. глубина
const int FIXED_TIME = 1; //фикс. время на ход
const int TOURNAMENT = 2; //турнирный режим

//Типы оценки в хеш-таблице
const int EMPTY = 0;
const int EXACT = 1;
const int ALPHA = 2;
const int BETA = 3;

// Таблицы PST

//Пешки
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

// Кони
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

// Слоны
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

// Ладьи
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

// Ферзи
const double queenMatr[BOARD_SIZE][BOARD_SIZE] {
{-20, -10, -10, -5, -5, -10, -10, -20},
{-10, 0, 0, 0, 0, 0, 0, -10},
{-10, 0, 5, 5, 5, 5, 0, -10},
{-5, 0, 5, 5, 5, 5, 0, -5},
{0, 0, 5, 5, 5, 5, 0, -5},
{-10, 0, 5, 0, 0, 0, 0, -10},
{-20, -10, -10, -5, -5, -10, -10, -20}
};

//Короли
const Score kingPST[BOARD_SIZE][BOARD_SIZE / 2] {
{S(-30, -50), S(-40, -40), S(-40, -30), S(-50, -20)},
{S(-30, -30), S(-40, -20), S(-40, -10), S(-50,   0)},
{S(-30, -30), S(-40, -10), S(-40,  20), S(-50,  30)},
{S(-30, -30), S(-40, -10), S(-40,  30), S(-50,  40)},
{S(-20, -30), S(-30, -10), S(-30,  30), S(-40,  40)},
{S(-10, -30), S(-20, -10), S(-20,  20), S(-20,  30)},
{S(20,  -30), S(20,  -30), S(0,     0), S(0,     0)},
{S(20,  -50), S(30,  -30), S(10,  -30), S(0,   -30)}
};

const int pstIndex[8] = {0, 1, 2, 3, 3, 2, 1, 0};

// Таблица отсечений Razoring (допуск отсечки) по глубине от 0-й до 9-й
const double RAZOR_MARGIN[10] = {0, 10, 50, 200, 500, 750, 1000, 1500, 2000, 2500};

// Бонусы
const double IsolatedPawnBonus = -15; // изолированные пешки
const double DoubleBishopsBonus = 30; // бонус за 2-х слонов
const double PassedPawnBonus[BOARD_SIZE] = {0, 10, 20, 30, 40, 60, 80, 0}; //Бонус проходных пешек
const double KnightMobilityBonus[9] = {-44, -32, -31, -4, 3, 7, 14, 17, 20};
const double BishopMobilityBonus[14] = {-31, -12, 5, 15, 18, 23, 26, 30, 32, 35, 38, 41, 44, 47};
const double RookMobilityBonus[15] = {-35, -30, -25, -20, -10, -5, 5, 10, 15, 23, 30, 36, 42, 50};
const double QueenMobilityBonus[28] = {-15, -10, 3, 4, 6, 9, 12, 15, 20, 25, 30, 35, 38, 41, 44, 47, 50, 52, 54, 56, 58, 60, 62, 64, 65, 66, 67, 68};
const double DualPawnBonus[9] = {0, 0, -5, -10, -15, -15, -15, -15, -15};

// Безопасность короля
const double king_security[8] = {8, 7, 6, 5, 4, 3, 2, 1};
const int SafetyTable[100] = {
		0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
	18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
	68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
	140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
	260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
	377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
	494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500
	};

const uint8_t colorExtended[2] = {BLACK, WHITE};

#endif
