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

bool SidePiece::isOwner(PlayerId id) const {
    return id == this->owner();
}

bool SidePiece::isPawn() const {
    switch(this->pieceType) {
        case SidePieceType::BlackSidePawn : 
        case SidePieceType::WhiteSidePawn :
            return true;
        default :
            return false;
    }
}

bool SidePiece::isQueen() const {
    switch(this->pieceType) {
        case SidePieceType::BlackSideQueen :
        case SidePieceType::WhiteSideQueen :
            return true;
        default :
            return false;
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