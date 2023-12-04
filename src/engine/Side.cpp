#include "engine/Side.hpp"
#include "utils/Cli.hpp"

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

std::string SidePosition::toString() const {
    return (this->horizontal ? "horizontal" : "vertical") + 
        Cli::to_string(this->sideVector);
};

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