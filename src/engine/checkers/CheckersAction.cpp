#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"

std::vector<CellPosition> const CheckersAction::authorizedOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

/*
    Is there any authorized/correct action ?
*/
bool CheckersAction::hasRemainingActions(
    const CheckersManager *manager,
    PlayerId author,
    uint step,
    Board board) {

	CellPieceType playerColor = author == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType rivalColor = author == 0 ? CellPieceType::BlackPawn : CellPieceType::WhitePawn;

     
    /* initialisation of the party */
    if (step < manager->players.size())
        return true;
    
    std::vector<CheckersAction> result;
    std::vector<CellPosition> pawns;
    for (int x = 0; x < 10; x++) for (int y = 0; y < 10; y++)
        if (board.getCell(x, y).pieceType == playerColor)
            pawns.push_back(CellPosition{x, y});

    for (auto pawn : pawns) for (auto offset : CheckersAction::authorizedOffsets) {
		if (offset.x == 2 || offset.y == -2) {
			CellPosition between = pawn + (offset)/2;
			CellPosition afterJump = pawn + offset;
			
			if (!board.isCaseInBoard(between) || !board.isCaseInBoard(afterJump))
				continue;
		
			if (!(board.isCaseEmpty(between) || board.getCell(between).pieceType == playerColor) &&
				board.isCaseEmpty(afterJump))
				return true;
		} else if (offset.x == 1 || offset.y == -1) {
			CellPosition afterJump = pawn + offset;
			if (!board.isCaseInBoard(afterJump))
				continue;
			if (board.isCaseEmpty(afterJump))
				return true;
		}
    }

    return false;
};

inline bool iff(bool const a, bool const b) {
    return (a && b) || (!a && !b);
};

bool CheckersAction::isValid(Board configuration) const { 
	if (jumps.size() == 0)
        return false;

    if (!iff(
        step < manager->players.size(),
        jumps.size() == 1
        )) return false;

    if (!configuration.isCaseInBoard(jumps[0]))
        return false;

    CellPieceType playerColor = configuration.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    CellPieceType rivalColor = configuration.player == 0 ? CellPieceType::BlackPawn : CellPieceType::WhitePawn;
    
    if (configuration.getCell(jumps[0]) != playerColor)
        return false;

    if (jumps.size() == 2) {
        if (!configuration.isCaseInBoard(jumps[1]))
            return false;
        CellPosition lastPosition = jumps[0];
        CellPosition currentPosition = jumps[1];
        CellPosition offset = currentPosition - lastPosition;
        if (offset == CellPosition(1,1) || offset == CellPosition(1,-1) || offset == CellPosition(-1,1) || offset == CellPosition(-1,-1)) {
            if (configuration.isCaseEmpty(currentPosition))
                return true;
            return false;
        } else {
            return false;
        }
    }

}