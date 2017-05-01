#ifndef FIGURECELL_HPP
#define FIGURECELL_HPP

#include <cstdint>

class FigureCell {
public:
  uint8_t figure, y, x;

  FigureCell();
  FigureCell(uint8_t _figure, uint8_t _y, uint8_t _x);
};

#endif
