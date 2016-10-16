#ifndef PV_HPP
#define PV_HPP

#include <vector>
#include "move.hpp"

class PV {
public:
    std::vector<Move>pv;
    double evalute;
    PV();
    PV(double eval);
    void push(Move mv);
    int size() const;
};

#endif
