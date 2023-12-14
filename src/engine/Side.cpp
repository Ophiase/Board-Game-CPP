#include "engine/Side.hpp"
#include "utils/Cli.hpp"

bool SidePiece::isNone() const {
    return this->pieceType == SidePieceType::NoneSide;
}

PlayerId SidePiece::owner() const {
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

bool SidePiece::operator==(SidePiece other) const {
    return this->pieceType == other.pieceType;
}

bool SidePiece::operator==(SidePieceType other) const {
    return this->pieceType == other;
}

bool SidePiece::operator!=(SidePiece other) const {
    return this->pieceType != other.pieceType;
}

bool SidePiece::operator!=(SidePieceType other) const {
    return this->pieceType != other;
}