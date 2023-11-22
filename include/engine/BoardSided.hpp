#pragma once

#include "Board.hpp"

class BoardSided : public Board {
    public :
        const std::vector<std::vector<SidePiece>> horizontalSidePieces;
        const std::vector<std::vector<SidePiece>> verticalSidePieces;
    protected :
        // Sub class const init
        BoardSided(
            std::vector<std::vector<CellPiece>> cellPieces, 
            std::vector<std::vector<SidePiece>> horizontalSidePieces,
            std::vector<std::vector<SidePiece>> verticalSidePieces,
            Player player) : 
                Board{cellPieces, player}, 
                horizontalSidePieces{horizontalSidePieces},
                verticalSidePieces{verticalSidePieces} 
                {};
    public:
        BoardSided(int dimension);
        ~BoardSided();

        // ---------------------------------------------------
        

        bool isSideEven (SidePosition v) const;
        bool isCaseInBoard(SidePosition v) const;
        bool isCaseEmpty(SidePosition v) const;
        SidePiece getCell(SidePosition v) const;
};