#pragma once

#include "engine/PieceInfo.hpp"

/*
    PlaceHolder, game defined.
*/
//template <class BoardType>
class Action {
    //static_assert(std::is_base_of<Board, BoardType>::value,
    //    "BoardType must inherit from Board");
    
    private:
    public:
        const Player author;
        Action(Player author) : author{author} {};
        
        //bool isValid(BoardType configuration);
        //BoardType apply(BoardType);
};