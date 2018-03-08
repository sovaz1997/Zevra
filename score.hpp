#ifndef SCORE_HPP
#define SCORE_HPP

class Score {
public:
    int mg, eg;
    Score(int _mg, int _eg);
    int getScore(int stageGame) const;
};

#endif