#include "score.hpp"

Score::Score(int _mg, int _eg) : mg(_mg), eg(_eg) {}

int Score::getScore(int stageGame) const {
    return mg * stageGame + (1 - stageGame) * eg;
}