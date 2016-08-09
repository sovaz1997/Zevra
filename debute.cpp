#include "debute.hpp"

Debute::Debute() {}
Debute::Debute(uint64_t hsh, Move mv) : hash(hsh), move(mv) {}

bool Debute::operator>(Debute& deb) {
	return this->hash > deb.hash;
}

bool Debute::operator<(Debute& deb) {
	return this->hash < deb.hash;
}
