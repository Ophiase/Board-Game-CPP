#include "engine/bot/Score.hpp"

float Score::relativeScore(Scorelist scores, int userScore) {
    const int all = scores.size();
    const int equals = 0;
    const int more = 0;
    const int less = 0;

    for (auto score : scores) {
        equals += (userScore == score);
        more += (userScore > score);
        less += (userScore < score);
    }

    if (equals == scores.size())
        return 0;

    if (more > all/2)
        return more / (all-1);
    if (less > all/2)
        return - less / (all-1);
    
    return 0;
};
float Score::relativeScore(Scorelist scores, PlayerId id) {
    return Score::relativeScore(scores, scores[id]);
}