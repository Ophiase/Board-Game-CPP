#pragma once

#include "engine/PieceInfo.hpp"

/*
    PlaceHolder, game defined.

    The following methods getActions / canPlayAction / applyAction / actionEquivalence
    requiere c++20 to be directly implemented in action. 
*/
class Action {
    
    private:
    public:
        const PlayerId author;
        Action(PlayerId author) : author{author} {};
};