#pragma once

#include "engine/type/ScoreList.hpp"
#include "engine/type/PlayerId.hpp"

class Score {
    public:
        /* 
            We assume the PlayerId corresponds 
        */
        static float relativeScore(Scorelist, PlayerId);
        
        /*
            Returns a value between -1 and 1,
            -1 if every scores are better
            +1 if every scores are equals
        */
        static float relativeScore(Scorelist, int);
};