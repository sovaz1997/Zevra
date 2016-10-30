#include "hash.hpp"

Hash::Hash() : enable(false) {}
Hash::Hash(uint64_t hsh, BitMove mv, int dpth, int pl, double eval, double a, double b, int type, double seval) : enable(true), hash(hsh), depth(dpth), ply(pl), alpha(a), beta(b), move(mv), evalute(eval), type_mv(type), staticEval(seval) {}
