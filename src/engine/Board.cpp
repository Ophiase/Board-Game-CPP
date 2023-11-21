#include "engine/Board.hpp"

bool Board::isCellEven (CellVector v) {
            return ((v.x + v.y) & 1) == 0;
        }

bool Board::isCaseInBoard(CellVector v) const {
    return (0 <= v.x) && (0 <= v.y)
        && (v.x < dimension) && (v.y < dimension);
}

bool Board::isCaseEmpty(CellVector v) const {
    return !getCell(v).has_value();
}

CellPiece Board::getCellValue(CellVector v) const {
    return getCell(v).value();
}

std::optional<CellPiece> Board::getCell(CellVector v) const {
    return cellPieces[v.y][v.x];
}