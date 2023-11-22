#include "engine/Side.hpp"

bool SidePiece::isNone() const {
    return this->pieceType == SidePieceType::NoneSide;
}

Player SidePiece::owner() const {
    switch(this->pieceType) {
        case SidePieceType::BlackSidePawn : 
        case SidePieceType::BlackSideQueen :
            return BlackPlayer;
        case SidePieceType::WhiteSidePawn : 
        case SidePieceType::WhiteSideQueen :
            return WhitePlayer;
        default :
            return NonePlayer;
    }
}