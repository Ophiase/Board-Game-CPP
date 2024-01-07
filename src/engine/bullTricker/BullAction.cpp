#include "engine/bullTricker/BullAction.hpp"
#include "engine/bullTricker/BullManager.hpp"
#include "engine/Combination.hpp"

const std::vector<CellPosition> BullAction::authorizedKingOffsets = {
	{1,0}, {0,1}, {-1,0}, {0,-1}
};

const std::vector<CellPosition> BullAction::surroundingCellsOffsets = {
	{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1}
};

std::vector<CellPosition> BullAction::getSurroundingCells(const BullState & state, CellPosition pos) const {
	std::vector<CellPosition> surroundingCells;
	for (CellPosition offset : surroundingCellsOffsets) {
		CellPosition newPos = pos + offset;
		if (state.board.isCaseInBoard(newPos))
			surroundingCells.push_back(newPos);
	}
	return surroundingCells;
}

bool BullAction::hasRemainingActions(const BullManager *, BullState state) {
	CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
	CellPieceType playerQueen = state.player == 0 ? CellPieceType::WhiteQueen : CellPieceType::BlackQueen;
	CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;

	(void)playerPawn;
	(void)playerQueen;
	(void)playerKing;

	throw NotImplemented();
	



	return false;
}

bool BullAction::isValidPawnAction(const BullState & state) const {
	bool status = state.player == 0 ? isValidWhitePawnAction(state) : isValidBlackPawnAction(state);
	return status;
}

bool BullAction::isValidWhitePawnAction(const BullState &) const {
	throw NotImplemented();
}

bool BullAction::isValidBlackPawnAction(const BullState &) const {
	throw NotImplemented();
}


bool BullAction::isValidQueenAction(const BullState &) const {
	throw NotImplemented();
}

bool BullAction::isValidKingAction(const BullState & state) const {
	CellPieceType rivalKing = state.player == 0 ? CellPieceType::BlackKing : CellPieceType::WhiteKing;
	(void)rivalKing;
	
	std::vector<CellPosition> surroundingCells = getSurroundingCells(state, jumps[1]);

	if (jumps.size() != 2)
			return false;

	for (CellPosition pos : surroundingCells) {
		CellPiece cell = state.board.getCell(pos);
		if (cell.isKing() && cell.owner() != state.player)
			return false;
	}
	
	CellPiece nextPos = state.board.getCell(jumps[1]);
	if (!nextPos.isNone())
		return false;
	CellPiece currentPos = state.board.getCell(jumps[0]);
	(void)currentPos;

	CellPosition offset = jumps[1] - jumps[0];
	if (std::find(authorizedKingOffsets.begin(), authorizedKingOffsets.end(), offset) == authorizedKingOffsets.end())
		return false;
	
	if (offset == CellPosition{1, 0}) {
        if (!state.board.verticalSidePieces[jumps[0].x + 1][jumps[0].y].isNone())
            return false;
    } else if (offset == CellPosition{0, 1}) {
        if (!state.board.horizontalSidePieces[jumps[0].x][jumps[0].y + 1].isNone())
            return false;
    } else if (offset == CellPosition{-1, 0}) {
		if (!state.board.verticalSidePieces[jumps[0].x][jumps[0].y].isNone())
			return false;
	} else if (offset == CellPosition{0, -1}) {
		if (!state.board.horizontalSidePieces[jumps[0].x][jumps[0].y].isNone())
			return false;
	}
	return true;
}

bool BullAction::isValid(BullState state) const {
	if (jumps.size() == 0 || jumps.size() == 1) 
        return false;

    for (auto jump : this->jumps)
        if (!state.board.isCaseInBoard(jump))
            return false;

    if (state.board.getCell(jumps[0]).owner() != this->author)
        return false;

    if (state.board.getCell(jumps[0]).isPawn())
        return isValidPawnAction(state);

    if (state.board.getCell(jumps[0]).isQueen())
        return isValidQueenAction(state);
	
	if (state.board.getCell(jumps[0]).isKing())
		return isValidKingAction(state);

    return false;
}

BullState BullAction::apply(BullState state) const {
	return state;
}

std::string BullAction::toString() const {
    return Cli::toString(jumps);
}