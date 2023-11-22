#pragma once

/*
    Forward declaration for Cell.hpp, Side.hpp Owning.hpp
*/

#define BlackPlayer 1
#define WhitePlayer 2
#define NonePlayer 0
using Player = int;

enum class CellPieceType {
    NoneCell,
    BlackPawn, WhitePawn, 
    BlackKing, WhiteKing, 
    YellowPawn, RedPawn
};

enum class SidePieceType {
    NoneSide,
    BlackSidePawn, WhiteSidePawn, 
    BlackSideQueen, WhiteSideQueen
};

class Piece {
    public:
        virtual bool isNone() const = 0;
        virtual Player owner() const = 0;
};