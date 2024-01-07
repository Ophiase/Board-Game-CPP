#include "engine/bot/Score.hpp"
#include <stdlib.h>

float Score::scoreToRelativeScorePosition(ScoreList scores, int userScore) {
    const int all = scores.size();
    uint equals = 0;
    uint more = 0;
    uint less = 0;

    for (auto score : scores) {
        equals += (userScore == score);
        more += (userScore > score);
        less += (userScore < score);
    }

    if (equals == scores.size())
        return 0;

    if (more > all/2)
        return (float)more / (float)(all-1);
    if (less > all/2)
        return -less / (float)(all-1);
    
    return 0;
};

float Score::idToRelativeScorePosition(ScoreList scores, PlayerId id) {
    return Score::scoreToRelativeScorePosition(scores, scores[id]);
}

float Score::zeroSumScore(ScoreList scores, PlayerId id) {
    float sum = -(float)scores[id];
    for (int score : scores)
        sum += score;
    return sum;
}