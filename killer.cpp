#include "killer.hpp"

Killer::Killer() : enable(false) {}
Killer::Killer(BitMove mv) : move(mv), enable(true) {}
