#pragma once

#include "engine/PieceInfo.hpp"

/*
    PlaceHolder, game defined.

    The following methods getActions / canPlay / applyAction / actionEquivalence
    requiere c++20 to be directly implemented in action. 
    
    Reason:
    Action need to be generic Action<BoardType>
    
    and automaticaly add
    to Manager
*/
class Action {
    
    private:
    public:
        const PlayerId author;
        Action(PlayerId author) : author{author} {};
};