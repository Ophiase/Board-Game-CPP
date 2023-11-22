#include "engine/BoardSided.hpp"

bool BoardSided::isSideEven (SidePosition v) const {
    return ((v.sideVector.x + v.sideVector.y) & 1) == 0;
}

bool BoardSided::isCaseInBoard(SidePosition v) const {
    if (v.sideVector.x < 0 || v.sideVector.y < 0)
        return false;
    
    if (v.horizontal)
        return 
            v.sideVector.x <= getDimension() && 
            v.sideVector.y <= getDimension();
    
    return
        v.sideVector.x < getDimension() && 
        v.sideVector.y <= getDimension();
}

bool BoardSided::isCaseEmpty(SidePosition v) const {
    return !getCell(v).isNone();
}

SidePiece BoardSided::getCell(SidePosition v) const {
    if (v.horizontal)
        return horizontalSidePieces[v.sideVector.y][v.sideVector.x];
    return verticalSidePieces[v.sideVector.y][v.sideVector.x];
}