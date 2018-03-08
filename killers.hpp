#ifndef KILLERS_HPP
#define KILLERS_HPP

#include "killer.hpp"

class Killers {
public:
    std::vector<Killer>whiteKiller;
	std::vector<Killer>blackKiller;
	std::vector<Killer>whiteSecondKiller;
	std::vector<Killer>blackSecondKiller;

    Killers();
};

#endif