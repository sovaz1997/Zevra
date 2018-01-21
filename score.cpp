#include "score.hpp"

Score::Score(double _mg, double _eg) : mg(_mg), eg(_eg) {}

double Score::getScore(double stageGame) const {
    return mg * stageGame + (1 - stageGame) * eg;
}