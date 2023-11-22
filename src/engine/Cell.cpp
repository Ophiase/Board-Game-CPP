#include "engine/Cell.hpp"

bool CellPiece::isNone () const {
    return this->pieceType == CellPieceType::NoneCell;
}

Player CellPiece::owner() const {
    switch(this->pieceType) {
        case CellPieceType::BlackPawn : 
        case CellPieceType::BlackKing :
            return BlackPlayer;

        case CellPieceType::WhitePawn : 
        case CellPieceType::WhiteKing:
            return WhitePlayer;
        
        default :
            return NonePlayer;
    }
}