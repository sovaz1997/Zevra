#ifndef BOARDINFO_HPP
#define BOARDINFO_HPP

#include <vector>
#include <cstdint>

class BoardInfo {
public:
  bool passant_enable;
	int passant_x, passant_y;
	bool whiteMove;

	bool blackShortCastleEnable;
	bool blackLongCastleEnable;
	bool whiteShortCastleEnable;
	bool whiteLongCastleEnable;

	std::vector<std::vector<uint8_t> > board;
	int numHalfMove;
	int move_rule_num;

  BoardInfo();
};

#endif
