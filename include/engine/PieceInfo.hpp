#pragma once

/*
    Forward declaration for Cell.hpp, Side.hpp Owning.hpp
*/

enum Player {
    Black, White, 
    None
};

enum CellPieceType {
    BlackPawn, WhitePawn, 
    BlackKing, WhiteKing, 
    YellowPawn, RedPawn
};

enum SidePieceType {
    BlackSidePawn, WhiteSidePawn, 
    BlackSideQueen, WhiteSideQueen
};
