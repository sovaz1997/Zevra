#include "score.hpp"

Score::Score(int _mg, int _eg) : mg(_mg), eg(_eg) {
    scoreCash.resize(64);
    
    for(int i = 0; i < scoreCash.size(); ++i) {
        scoreCash[i] = getScore(std::min(1.0, i / 32.0));
    }
}

int Score::getScore(double stageGame) const {
    return mg * stageGame + (1 - stageGame) * eg;
}