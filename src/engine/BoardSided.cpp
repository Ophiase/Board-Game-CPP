#include "engine/BoardSided.hpp"
#include "utils/NotImplemented.hpp"

bool BoardSided::isSideEven (SidePosition v) const {
    return ((v.sideVector.x + v.sideVector.y) & 1) == 0;
}

bool BoardSided::isCaseInBoard(SidePosition v) const {
    if (v.sideVector.x < 0 || v.sideVector.y < 0)
        return false;
    
    if (v.horizontal)
        return 
            v.sideVector.x <= (int)getDimension() && 
            v.sideVector.y <= (int)getDimension();
    
    return
        v.sideVector.x < (int)getDimension() && 
        v.sideVector.y <= (int)getDimension();
}

bool BoardSided::isCaseEmpty(SidePosition v) const {
    return getCell(v).isNone();
}

SidePiece BoardSided::getCell(SidePosition v) const {
    if (v.horizontal)
        return horizontalSidePieces[v.sideVector.y][v.sideVector.x];
    return verticalSidePieces[v.sideVector.y][v.sideVector.x];
}

bool BoardSided::operator==(BoardSided other) const {
    return Board::operator==(other) && 
        (this->horizontalSidePieces == other.horizontalSidePieces) &&
        (this->verticalSidePieces   == other.verticalSidePieces);
}

bool BoardSided::operator!=(BoardSided other) const {
    return (*this) != other;
}

std::string BoardSided::toString() const {
    throw NotImplemented();
}