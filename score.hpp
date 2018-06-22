#ifndef SCORE_HPP
#define SCORE_HPP

#include <vector>

class Score {
public:
    int mg, eg;
    Score(int _mg, int _eg);
    int getScore(double stageGame) const;
    std::vector<int>scoreCash;
};

#endif