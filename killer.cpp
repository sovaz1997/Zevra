#include "killer.hpp"

Killer::Killer() : enable(false) {}
Killer::Killer(Move mv) : move(mv), enable(true) {}
