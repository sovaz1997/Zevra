#include "hash.hpp"

Hash::Hash() : enable(false) {}
Hash::Hash(uint64_t hsh, Move mv, int dpth, double eval, int type) : enable(true), hash(hsh), depth(dpth), move(mv), evalute(eval), type_mv(type) {}
