#include "engine/bulltricker/BullAction.hpp"
#include "engine/bulltricker/BullManager.hpp"
#include "engine/Combination.hpp"

const std::vector<CellPosition> BullAction::authorizedKingOffsets = {
	{1,0}, {0,1}, {-1,0}, {0,-1}
};

const std::vector<CellPosition> BullAction::surroundingCellsOffsets = {
	{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1}
};

std::vector<CellPosition> BullAction::getSurroundingCells(const BullState & state, CellPosition pos) {
	std::vector<CellPosition> surroundingCells;
	for (CellPosition offset : surroundingCellsOffsets) {
		CellPosition newPos = pos + offset;
		if (state.board.isCaseInBoard(newPos))
			surroundingCells.push_back(newPos);
	}
	return surroundingCells;
}

bool BullAction::hasRemainingActions(const BullManager * manager, BullState state) {
	return !BullAction::getActions(manager, state).empty();
}

void BullAction::completeSpecificKingActions(
    const BullManager *manager, 
    const BullState & state,

    std::vector<CellPath> & visited,
    std::vector<CellPath> & nextVisited,
    CellPath currentPath
) {
	auto *board = &state.board;
    auto initPosition = currentPath[0];

	for (auto offset : authorizedKingOffsets) {
        auto toPosition = initPosition + offset;
		if (!board->isCaseInBoard(toPosition))
			continue;

		std::vector<CellPosition> surroundingCells = getSurroundingCells(state, toPosition);
		for (CellPosition pos : surroundingCells) {
			CellPiece cell = state.board.getCell(pos);
			if (cell.isKing() && cell.owner() != state.player)
				continue;
		}
		if (offset == CellPosition{1, 0}) {
			if (!state.board.verticalSidePieces[initPosition.x + 1][initPosition.y].isNone())
				continue;
		} else if (offset == CellPosition{0, 1}) {
			if (!state.board.horizontalSidePieces[initPosition.x][initPosition.y + 1].isNone())
				continue;
		} else if (offset == CellPosition{-1, 0}) {
			if (!state.board.verticalSidePieces[initPosition.x][initPosition.y].isNone())
				continue;
		} else if (offset == CellPosition{0, -1}) {
			if (!state.board.horizontalSidePieces[initPosition.x][initPosition.y].isNone())
				continue;
		}

		if (!board->getCell(toPosition).isNone())
			continue;

		CellPath next = currentPath;
        next.push_back(toPosition);

        nextVisited.push_back(next);
	}
}

std::vector<BullAction> BullAction::getSpecificActions(
	const BullManager * manager, const BullState& state, CellPosition axiom) {
	
	std::vector<CellPath> visited{};
    std::vector<CellPath> nextVisited{CellPath{axiom}};

	int depth = 0;

    do {
        depth++;
        visited = nextVisited;
        nextVisited.clear();
		for (auto path : visited)
			completeSpecificKingActions(
				manager, state, 
				visited,
				nextVisited,
				path
			);
	} while (!nextVisited.empty() && depth < 2);
	if (depth == 1)
        return std::vector<BullAction>{};

	std::vector<BullAction> actions{};

	for (auto path : visited)
        actions.push_back(BullAction{
            manager, state.player, state.step, path
        });

    return actions;
}

std::vector<BullAction> BullAction::getSpecificActions(
    const BullManager * manager, const BullState&, SidePosition axiom) {
		throw NotImplemented();
}

std::vector<BullAction> BullAction::getActions(const BullManager * manager, BullState state) {
	const int dimension = (int)state.board.getDimension();
    const BoardSided *board = &state.board;
    std::vector<BullAction> actions{};

    for (int x = 0; x < dimension; x++)
    for (int y = 0; y < dimension; y++)
    if (board->getCell(x, y).owner() == board->player) {
        
        auto actionsToAdd = getSpecificActions(
            manager,
            state,
            CellPosition{x, y});
        
        for (auto action : actionsToAdd)
            actions.push_back(action);
    }
	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 7; y++){
			if (board->verticalSidePieces[x][y].owner() == board->player) {
				auto actionsToAdd = getSpecificActions(
					manager,
					state,
					SidePosition({x, y},0));
				for (auto action : actionsToAdd)
					actions.push_back(action);
			} if (board->horizontalSidePieces[y][x].owner() == board->player) {
				auto actionsToAdd = getSpecificActions(
					manager,
					state,
					SidePosition({y,x},1));
				for (auto action : actionsToAdd)
					actions.push_back(action);
			}
		}
	}
    return actions;
}

bool BullAction::isValidWhitePawnAction(const BullState & state) const {
	return false;
}

bool BullAction::isValidBlackPawnAction(const BullState & state) const {
	return false;
}

bool BullAction::isValidPawnAction(const BullState & state) const {
	bool status = state.player == 0 ? isValidWhitePawnAction(state) : isValidBlackPawnAction(state);
	return status;
}

bool BullAction::isValidQueenAction(const BullState & state) const {
	return false;
}

bool BullAction::isValidKingAction(const BullState & state) const {
	auto actions = BullAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

    return false;
}

bool BullAction::isValid(BullState state) const {
	if (isSidePath) {
		if (sideJumps.size() == 0 || sideJumps.size() == 1)
			return false;
		for (auto jump : this->sideJumps)
			if (!state.board.isCaseInBoard(jump))
				return false;
		if (state.board.getCell(sideJumps[0]).owner() != this->author)
			return false;
		if (state.board.getCell(sideJumps[0]).isPawn())
			return isValidPawnAction(state);
		if (state.board.getCell(sideJumps[0]).isQueen())
			return isValidQueenAction(state);
	} else {
		if (cellJumps.size() == 0 || cellJumps.size() == 1) 
			return false;
		for (auto jump : this->cellJumps)
			if (!state.board.isCaseInBoard(jump))
				return false;
		if (state.board.getCell(cellJumps[0]).owner() != this->author)
			return false;
		if (state.board.getCell(cellJumps[0]).isKing())
			return isValidKingAction(state);
	}
	return false;
}

BullState BullAction::apply(BullState state) const {
	return state;
}

std::string BullAction::toString() const {
	if (isSidePath)
    	return Cli::toString(sideJumps);
	else
		return Cli::toString(cellJumps);
}