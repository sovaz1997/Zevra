#include "hash.hpp"

Hash::Hash() : enable(false) {}
Hash::Hash(uint64_t hsh, BitMove mv, int dpth, int pl, double eval, double a, double b, int type, double seval) : enable(true), hash(hsh), move(mv), depth(dpth), ply(pl), evalute(eval), alpha(a), beta(b), type_mv(type), staticEval(seval) {}
