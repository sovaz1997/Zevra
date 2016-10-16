#include "pv.hpp"

PV::PV() {}
PV::PV(double eval) : evalute(eval) {}

void PV::push(Move mv) {
    pv.insert(pv.begin(), mv);
}

int PV::size() const {
    return pv.size();
}
