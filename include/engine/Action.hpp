#pragma once

#include "engine/PieceInfo.hpp"

/*
    PlaceHolder, game defined.
*/
class Action {
    
    private:
    public:
        const Player author;
        Action(Player author) : author{author} {};
};