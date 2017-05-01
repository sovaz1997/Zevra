#include "extendedmove.hpp"


ExtendedMove::ExtendedMove(BitMove mv, int64_t eval) : move(mv), evalute(eval) {}

bool ExtendedMove::operator<(ExtendedMove& right) {
	return (this->evalute < right.evalute);
}

bool ExtendedMove::operator>(ExtendedMove& right) {
	return (this->evalute > right.evalute);
}
