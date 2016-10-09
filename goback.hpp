#ifndef GOBACK_HPP
#define GOBACK_HPP

#include <cstdint>
#include "constants.hpp"
#include "board.hpp"
#include "figurecell.hpp"

class Board;

class GoBack {
public:
  GoBack();
  //new
  FigureCell fig1, fig2, fig3;
  bool passant = false;
  bool passant_enable;

  bool whiteShortCastleEnable;
  bool whiteLongCastleEnable;
  bool blackShortCastleEnable;
  bool blackLongCastleEnable;

  uint16_t numHalfMove;
  uint16_t move_rule_num;
  uint8_t type;
  uint8_t passant_y;
  uint8_t passant_x;

  void goBack(Board* board);
};

#endif
