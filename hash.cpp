#include "hash.hpp"

Hash::Hash() : enable(false) {}
Hash::Hash(uint64_t hsh, Move mv, int dpth, double eval) : enable(true), hash(hsh), depth(dpth), move(mv), evalute(eval) {}
