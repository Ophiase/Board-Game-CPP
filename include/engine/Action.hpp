#pragma once

#include "engine/PieceInfo.hpp"

/*
    PlaceHolder, game defined.

    The following methods getActions / canPlayAction / applyAction / actionEquivalence
    requiere c++20 to be directly implemented in action.

    To put action logic back in the class action instead of manager, we need to remove
    genericity.
*/
class Action {
    
    private:
    public:
        const PlayerId author;
        Action(PlayerId author) : author{author} {};
};