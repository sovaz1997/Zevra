#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <vector>
#include <cstdint>
#include <cwchar>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "score.hpp"

//#define S SCORE
using S = Score;

bool is_input_available();

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

const double k = 1; //коэффициент масштабирования

const double PAWN_EV = 100 * k;
const double KNIGHT_EV = 320 * k;
const double BISHOP_EV = 330 * k;
const double ROOK_EV = 550 * k;
const double QUEEN_EV = 1100 * k;

const double ENDGAME_PAWN_EV = 120 * k;
const double ENDGAME_QUEEN_EV = 1000 * k;
const double ENDGAME_KNIGHT_EV = 330 * k;


//Типы ходов
const int FIXED_DEPTH = 0; //фикс. глубина
const int FIXED_TIME = 1; //фикс. время на ход
const int TOURNAMENT = 2; //турнирный режим

const int REAL_EV = 0;
const int BETA_CUT_EV = 1;
const int ALPHA_CUT_EV = 2;

//Типы оценки в хеш-таблице
const int EMPTY = 0;
const int EXACT = 1;
const int ALPHA = 2;
const int BETA = 3;

const int MIN_DEPTH = 2;

// Веса всех фигур
const double ALL_MATERIAL = 16 * PAWN_EV
+	4 * KNIGHT_EV
+	4 * BISHOP_EV
+	4 * ROOK_EV
+	QUEEN_EV;

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

// Бонусы
const double PASSED_PAWN_BONUS = 50; // проходная пешка
const double DUAL_PAWN_BONUS = -15; // сдвоенные пешки
const double ISOLATED_PAWN_BONUS = -15; // изолированные пешки
const double FORPOST_BONUS = 5; //форпост для коней (не исп.)
const double promotePawnBonus[BOARD_SIZE] = {0, 10, 20, 30, 40, 60, 80, 0}; //Бонус проходных пешек (вместо таблицы PST)

// Безопасность короля
const double attack_bonus[7] = {0, 1, 3, 6, 10, 14, 20};
const double king_security[8] = {8, 7, 6, 5, 4, 3, 2, 1};

// Таблица отсечений Razoring (допуск отсечки) по глубине от 0-й до 9-й
const double RAZOR_MARGIN[10] = {0, 10, 50, 200, 500, 750, 1000, 1500, 2000, 2500};

#endif
