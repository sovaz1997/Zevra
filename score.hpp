#ifndef SCORE_HPP
#define SCORE_HPP

class Score {
public:
    double mg, eg;
    Score(double _mg, double _eg);
    double getScore(double stageGame) const;
};

#endif