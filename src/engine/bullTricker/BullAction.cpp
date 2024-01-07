#include "engine/bullTricker/BullAction.hpp"
#include "engine/bullTricker/BullManager.hpp"
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

void BullAction::completeSpecificPawnActions(
	const BullManager * manager, 
	const BullState & state,

	std::vector<SidePath> & visited,
	std::vector<SidePath> & nextVisited,
	SidePath currentPath
){}

void BullAction::completeSpecificQueenActions(
	const BullManager * manager, 
	const BullState & state,

	std::vector<SidePath> & visited,
	std::vector<SidePath> & nextVisited,
	SidePath currentPath
){}

void BullAction::completeSpecificKingActions(
    const BullManager *manager, 
    const BullState & state,

    std::vector<CellPath> & visited,
    std::vector<CellPath> & nextVisited,
    CellPath currentPath
) {
	(void)visited;
	(void)manager;

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
	const BullManager * manager, const BullState& state, CellPosition axiom) 
{
	(void)axiom;
	
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
    const BullManager * manager, const BullState& state, SidePosition axiom) {
	
	std::vector<SidePath> visited{};
    std::vector<SidePath> nextVisited{SidePath{axiom}};

    const bool isPawn = state.board.getCell(axiom).isPawn();

    int depth = 0;

    do {
        depth++;
        visited = nextVisited;
        nextVisited.clear();

        for (auto path : visited)
            if (isPawn)
                completeSpecificPawnActions(
                    manager, state, 
                    
                    visited,
                    nextVisited,
                    path
                );
            else
                completeSpecificQueenActions(
                    manager, state, 
                    
                    visited,
                    nextVisited,
                    path
                );
    } while(!nextVisited.empty() && (isPawn || (depth <= manager->MAX_QUEEN_DEPTH)));

    if (depth == 1)
        return std::vector<BullAction>{};
    
    std::vector<BullAction> actions{};

    for (auto path : visited)
        actions.push_back(BullAction{
            manager, state.player, state.step, path
        });

    return actions;
}

std::vector<BullAction> BullAction::getPawnMoves(
    const BullManager * manager, const BullState&) {
	throw NotImplemented();
}

std::vector<BullAction> BullAction::getQueenMoves(
    const BullManager * manager, const BullState&) {
	throw NotImplemented();
}

std::vector<BullAction> BullAction::getCaptures(
    const BullManager *manager, const BullState & state) 
{
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

std::vector<BullAction> BullAction::getActions(const BullManager * manager, BullState state) {
	auto captures = getCaptures(manager, state);

    int maxCapture = 0;
    for (auto capture : captures)
        maxCapture = std::max(
			std::max(maxCapture, (int)capture.cellJumps.size()),
			std::max(maxCapture, (int)capture.sideJumps.size())
		);

    if (maxCapture > 0) { 
        // if player can capture, he's forced to capture
        std::vector<BullAction> actions{};
        for (auto capture : captures)
            if (
                ((int)capture.cellJumps.size() == maxCapture) ||
				((int)capture.sideJumps.size() == maxCapture)
                // state.board.getCell(capture.jumps[0]).isQueen() 
            )
                actions.push_back(capture);

        return actions;
	}

    auto moves = getPawnMoves(manager, state);
    auto queenMoves = getQueenMoves(manager, state);
    for (auto queenMove : queenMoves)
        moves.push_back(queenMove);

    return moves;
}

bool BullAction::isValidQueenCapture(const BullState & state) const {
	return false;
}


bool BullAction::isValidPawnAction(const BullState & state) const {
	auto actions = BullAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;
    return false;
}

bool BullAction::isValidQueenAction(const BullState & state) const {
	/* 
        The number of queen's capture is very high. So getActions limit the
        number of queen's capture computed. We need to verify
        separatly that a queen's capture is valid.
    */

    if (isValidQueenCapture(state))
        return true;

    // We then verify if it's a queen's move.
    auto actions = BullAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

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
	auto cells = state.board.cellPieces;
	auto sidesH = state.board.horizontalSidePieces;
	auto sidesV = state.board.verticalSidePieces;
	
	PlayerId nextPlayer = state.player == 0 ? 1 : 0;

	if (!isSidePath) {
		// king move :
		auto first = cellJumps[0];
		auto second = cellJumps[1];

		cells[second.y][second.x].pieceType = cells[first.y][first.x].pieceType;
		cells[first.y][first.x].pieceType = CellPieceType::NoneCell;

		BoardSided nextBoard{
			cells, sidesH, sidesV, nextPlayer
		};

		return BullState{nextBoard, state.scores, state.step+1, nextPlayer};
	}

	const auto first = sideJumps[0];
	const auto firstV = first.sideVector; // V for vector here (not horizontal)
	const auto last = sideJumps[sideJumps.size()-1];
	const auto lastV = last.sideVector;

	int bonus = 0;

	if (first.horizontal && last.horizontal) {
		// PROMOTION
		if (author == WhitePlayer && last.sideVector.y == 0)
			sidesH[firstV.y][firstV.x].pieceType = SidePieceType::WhiteSideQueen;
		if (author == BlackPlayer && last.sideVector.y == 7)
			sidesH[firstV.y][firstV.x].pieceType = SidePieceType::BlackSideQueen;
	}

	if (first.horizontal == last.horizontal) {
		auto dir = lastV - firstV;
		auto position = firstV;
		
		if (dir.x > 0) dir.x = 1;
		else if (dir.x < 0) dir.x = -1;
		else if (dir.y > 0) dir.y = 1;
		else if (dir.y < 0) dir.y = -1;

		if (first.horizontal) { // HORIZONTAL
			sidesH[lastV.y][lastV.x].pieceType = sidesH[firstV.y][firstV.x].pieceType;
			sidesH[position.y][position.x] = SidePieceType::NoneSide;
			position += dir;

			while (position != lastV) {
				if (state.board.getCell(SidePosition{position, true}).pieceType != SidePieceType::NoneSide)
					bonus++;
				sidesH[position.y][position.x] = SidePieceType::NoneSide;
				position += dir;
			}
		} else { // VERTICAL
			sidesV[lastV.y][lastV.x].pieceType = sidesV[firstV.y][firstV.x].pieceType;
			sidesV[position.y][position.x] = SidePieceType::NoneSide;
			position += dir;

			while (position != lastV) {
				if (state.board.getCell(SidePosition{position, false}).pieceType != SidePieceType::NoneSide)
					bonus++;
				sidesV[position.y][position.x] = SidePieceType::NoneSide;
				position += dir;
			}
		}
	} else {
		if (first.horizontal) {
			sidesV[lastV.y][lastV.x].pieceType = sidesH[firstV.y][firstV.x].pieceType;
			sidesH[firstV.y][firstV.x].pieceType = SidePieceType::NoneSide;
		} else {
			sidesH[lastV.y][lastV.x].pieceType = sidesV[firstV.y][firstV.x].pieceType;
			sidesV[firstV.y][firstV.x].pieceType = SidePieceType::NoneSide;
		}
	}

		BoardSided nextBoard{
		cells, sidesH, sidesV, nextPlayer
	};

	ScoreList scores = state.scores;
	scores[author] += bonus;

	return BullState{nextBoard, scores, state.step+1, nextPlayer};
}

std::string BullAction::toString() const {
	if (isSidePath)
    	return Cli::toString(sideJumps);
	else
		return Cli::toString(cellJumps);
}