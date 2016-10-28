#include "hash.hpp"

Hash::Hash() : enable(false) {}
Hash::Hash(uint64_t hsh, BitMove mv, int dpth, double eval, double a, double b, int type) : enable(true), hash(hsh), depth(dpth), alpha(a), beta(b), move(mv), evalute(eval), type_mv(type) {}
