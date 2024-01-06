#pragma once

#include "Board.hpp"
#include "type/SidePosition.hpp"

class BoardSided : public Board {
    public :
        const std::vector<std::vector<SidePiece>> horizontalSidePieces;
        const std::vector<std::vector<SidePiece>> verticalSidePieces;        
    public:
        BoardSided(int dimension);
        BoardSided(
            std::vector<std::vector<CellPiece>> cellPieces, 
            std::vector<std::vector<SidePiece>> horizontalSidePieces,
            std::vector<std::vector<SidePiece>> verticalSidePieces,
            PlayerId player) : 
                Board{cellPieces, player}, 
                horizontalSidePieces{horizontalSidePieces},
                verticalSidePieces{verticalSidePieces} 
                {};
        
        bool operator==(const BoardSided &) const;
        bool operator!=(const BoardSided &) const;

        // ---------------------------------------------------
        

        bool isSideEven (SidePosition v) const;
        bool isCaseInBoard(SidePosition v) const;
        bool isCaseEmpty(SidePosition v) const;
        SidePiece getCell(SidePosition v) const;

        std::string toString() const;
};