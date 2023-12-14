#pragma once

#include "engine/type/PlayerId.hpp"
#include <vector>

/*
    PlaceHolder, game defined.

    Need to be reworked.
*/
class Action {  
    private:
    public:
        const PlayerId author;
        Action(PlayerId author) : author{author} {};
};