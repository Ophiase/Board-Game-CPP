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

bool CellPiece::operator==(CellPiece other) {
    return this->pieceType == other.pieceType;
}

bool CellPiece::operator==(CellPieceType other) {
    return this->pieceType == other;
}

bool CellPiece::operator!=(CellPiece other) {
    return this->pieceType != other.pieceType;
}

bool CellPiece::operator!=(CellPieceType other) {
    return this->pieceType != other;
}