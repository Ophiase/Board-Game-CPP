#include "engine/bulltricker/BullAction.hpp"
#include "engine/bulltricker/BullManager.hpp"
#include "engine/loot/Combination.hpp"

bool BullAction::hasRemainingActions(const BullManager *, BullState state) {
	CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
	CellPieceType playerQueen = state.player == 0 ? CellPieceType::WhiteQueen : CellPieceType::BlackQueen;
	CellPieceType playerKing = state.player == 0 ? CellPieceType::WhiteKing : CellPieceType::BlackKing;

	



	return false;
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

    return false;
}

BullState BullAction::apply(BullState state) const {
	return state;
}

std::string BullAction::toString() const {
    return Cli::toString(jumps);
}