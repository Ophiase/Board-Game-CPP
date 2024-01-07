#include "engine/Cell.hpp"
#include "utils/Cli.hpp"

bool CellPiece::isNone () const {
    return this->pieceType == CellPieceType::NoneCell;
}

bool CellPiece::isKing() const {
    switch(this->pieceType) {
        case CellPieceType::BlackKing :
        case CellPieceType::WhiteKing :
            return true;
        default :
            return false;
    }
}

bool CellPiece::isQueen() const {
    switch(this->pieceType) {
        case CellPieceType::BlackQueen :
        case CellPieceType::WhiteQueen :
            return true;
        default :
            return false;
    }
}

bool CellPiece::isPawn() const {
    return !isNone() && !isKing() && !isQueen();
}

PlayerId CellPiece::owner() const {
    switch(this->pieceType) {
        case CellPieceType::BlackPawn : 
        case CellPieceType::BlackKing :
        case CellPieceType::BlackQueen :
            return BlackPlayer;

        case CellPieceType::WhitePawn : 
        case CellPieceType::WhiteKing :
        case CellPieceType::WhiteQueen :
            return WhitePlayer;
        
        default :
            return NonePlayer;
    }
}

bool CellPiece::isOwner(PlayerId id) const {
    return id == this->owner();
}

std::string CellPiece::toString() const {
    return "[" + std::to_string((int)this->pieceType) + "]";
}

bool CellPiece::operator==(CellPiece other) const {
    return this->pieceType == other.pieceType;
}

bool CellPiece::operator==(CellPieceType other) const {
    return this->pieceType == other;
}

bool CellPiece::operator!=(CellPiece other) const {
    return this->pieceType != other.pieceType;
}

bool CellPiece::operator!=(CellPieceType other) const {
    return this->pieceType != other;
}
