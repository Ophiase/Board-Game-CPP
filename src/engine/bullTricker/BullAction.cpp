#include "engine/bullTricker/BullAction.hpp"
#include "engine/bullTricker/BullManager.hpp"
#include "engine/Combination.hpp"

const std::vector<CellPosition> BullAction::authorizedKingOffsets = {
	{1,0}, {0,1}, {-1,0}, {0,-1}
};

const std::vector<CellPosition> BullAction::surroundingCellsOffsets = {
	{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1}
};

bool BullAction::revEqual(const BullAction &other) {
	if (this->surrend && other.surrend) return true;
	if (this->isSidePath)
        return this->sideJumps == std::vector<SidePosition>(
			other.sideJumps.rbegin(), other.sideJumps.rend());

    return this->cellJumps == std::vector<CellPosition>(
		other.cellJumps.rbegin(), other.cellJumps.rend());

}

bool BullAction::operator==(const BullAction &other) {
	if (this->surrend && other.surrend) return true;
	if (this->isSidePath)
		return this->sideJumps == other.sideJumps;
	return this->cellJumps == other.cellJumps;
}

bool BullAction::operator!=(const BullAction &other) {
	return !(*this == other);
}

BullAction BullAction::getPat(const BullManager *manager, PlayerId author, uint step) {
	return BullAction{manager, author, step, false, true};
}

BullAction BullAction::getSurrend(const BullManager *manager, PlayerId author, uint step) {
	return BullAction{manager, author, step, true, false};
}

// -----------------------------------------------------------------------

bool BullAction::isSurrounded(const BullManager*, BullState state, CellPosition pos) {
	std::vector<SidePiece> pieces;
	int nbPlayerPieces = 0;
	int nbRivalPieces = 0;

	pieces.push_back(state.board.verticalSidePieces[pos.y][pos.x + 1]);
	pieces.push_back(state.board.horizontalSidePieces[pos.y + 1][pos.x]);
	pieces.push_back(state.board.verticalSidePieces[pos.y][pos.x]);
	pieces.push_back(state.board.horizontalSidePieces[pos.y][pos.x]);

	for (auto piece: pieces) {
		if (piece.owner() == state.player)
			nbPlayerPieces++;
		else if (piece.pieceType != SidePieceType::NoneSide)
			nbRivalPieces++;
	}
	if (nbPlayerPieces + nbRivalPieces < 4)
		return false;
	if (nbRivalPieces == 0)
		return false;
	return true;
}

bool BullAction::hasRemainingActions(const BullManager *manager, BullState state) {

	//Check if a mate has been made
	CellPosition playerKing;
	CellPosition opponentKing;
	for (int x = 0; x < 7; x++)
	for (int y = 0; y < 7; y++)
		if (state.board.getCell(x, y).owner() == NonePlayer)
			continue;
		else if (state.board.getCell(x, y).owner() == state.player)
			playerKing = CellPosition{x, y};
		else
			opponentKing = CellPosition{x, y};

	if (isSurrounded(manager, state, playerKing) || isSurrounded(manager, state, opponentKing))
		return false;

	//Check if the two players still have enough pieces
	int playerSidePieces = 0;
	int rivalSidePieces = 0;	
	for (int x = 0; x < 7; x++){
		for (int y = 0; y < 8; y++){
			if (state.board.horizontalSidePieces[y][x].owner() == state.player)
				playerSidePieces++;
			else if (state.board.horizontalSidePieces[y][x].pieceType != SidePieceType::NoneSide)
				rivalSidePieces++;
			if (state.board.verticalSidePieces[x][y].owner() == state.player)
				playerSidePieces++;
			else if (state.board.verticalSidePieces[x][y].pieceType != SidePieceType::NoneSide)
				rivalSidePieces++;
		}
	}
	if (playerSidePieces <= 3 && rivalSidePieces <= 3)
		return false;
		
	return !BullAction::getActions(manager, state).empty();
}

// -----------------------------------------------------------------------

// return true if there is captureKing
bool BullAction::isThereAKingCapture(
	BullAction action, CellPosition kingPosition) {
	
	if (!action.isSidePath)
		return false;
	
	if (action.sideJumps[0].horizontal != action.sideJumps[1].horizontal)
		return false;
	
	
	for (uint i = 1; i < action.sideJumps.size(); i++) {
		auto from = action.sideJumps[i-1];
		auto to = action.sideJumps[i];
		auto fromV = from.sideVector;
		auto toV = to.sideVector;

		auto diff = toV - fromV;
		diff.x = diff.x > 0 ? 1 : (diff.x < 0 ? -1 : 0);
		diff.y = diff.y > 0 ? 1 : (diff.y < 0 ? -1 : 0);
		
		if (from.horizontal) {
			if (diff.y == 0)
				return false;
			if (kingPosition.x != fromV.x)
				return false;
		} else {
			if (diff.x == 0)
				return false;
			if (kingPosition.y != fromV.y)
				return false;
		}

		// todo refactor
			// float kingProjectedAlongAxis
			// float minAlongAxis
			// float maxAlongAxis
			// then it can be done in 1 if
		if (from.horizontal) {
			if (diff.y > 0) {
				if (fromV.y <= kingPosition.y && kingPosition.y < toV.y)
					return true;
			} else {
				if (toV.y <= kingPosition.y && kingPosition.y < fromV.y)
					return true;
			}
		} else {
			if (diff.x > 0) {
				if (fromV.x <= kingPosition.x && kingPosition.x < toV.x)
					return true; 
			} else {
				if (toV.x <= kingPosition.x && kingPosition.x < fromV.x)
					return true;
			}
		}
	}

	return false;
}

void BullAction::getQueenStraightMove(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions,
	const SideVector dir
) {
	auto *board = &state.board;

	SidePosition to = position;
	to.sideVector += dir;

	while (board->isCaseInBoard(to) && board->getCell(to).isNone()) {
		actions.push_back(BullAction{
			manager, state.player, state.step+1, SidePath{
				position, to
			}
		});
		
		to.sideVector += dir;
	} 
}


void BullAction::getQueenHorizontalMoves(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions
) {
	getQueenStraightMove(manager, state, position, actions, SideVector{0, 1});
	getQueenStraightMove(manager, state, position, actions, SideVector{0, -1});

	getQueenStraightMove(manager, state, position, actions, SideVector{1, 0});
	getQueenStraightMove(manager, state, position, actions, SideVector{-1, 0});

	std::vector<SidePosition> toTryList{
		SidePosition{position.sideVector + SideVector{0, 0}, false},
		SidePosition{position.sideVector + SideVector{1, 0}, false},
		SidePosition{position.sideVector + SideVector{0, -1}, false},
		SidePosition{position.sideVector + SideVector{1, -1}, false}
	};

	for (auto land : toTryList)
		if (
			state.board.isCaseInBoard(land) && 
			state.board.getCell(land).isNone()
		)
			actions.push_back(BullAction{
			manager, state.player, state.step+1, SidePath{
				position, land
			}
		});
}

void BullAction::getQueenVerticalMoves(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions
) {
	getQueenStraightMove(manager, state, position, actions, SideVector{1, 0});
	getQueenStraightMove(manager, state, position, actions, SideVector{-1, 0});

	getQueenStraightMove(manager, state, position, actions, SideVector{0, 1});
	getQueenStraightMove(manager, state, position, actions, SideVector{0, -1});

	std::vector<SidePosition> toTryList{
		SidePosition{position.sideVector + SideVector{ 0, 0}, true},
		SidePosition{position.sideVector + SideVector{-1, 0}, true},
		SidePosition{position.sideVector + SideVector{ 0, 1}, true},
		SidePosition{position.sideVector + SideVector{-1, 1}, true}
	};

	for (auto land : toTryList)
		if (
			state.board.isCaseInBoard(land) && 
			state.board.getCell(land).isNone()
		)
			actions.push_back(BullAction{
			manager, state.player, state.step+1, SidePath{
				position, land
			}
		});
}

void BullAction::getKingMoves(
	const BullManager *manager, const BullState & state,
	CellPosition position, std::vector<BullAction> & actions
) {
	PlayerId opponent = state.player == WhitePlayer ? 
		BlackPlayer : WhitePlayer;

	CellPosition opponentPosition;
	for (opponentPosition.x = 0; opponentPosition.x < (int)state.board.getDimension(); opponentPosition.x++)
	for (opponentPosition.y = 0; opponentPosition.y < (int)state.board.getDimension(); opponentPosition.y++)
		if (state.board.getCell(opponentPosition).isOwner(opponent))
			break;

	std::vector<CellPosition> const toTryList{
		position + CellPosition{0, 1},
		position + CellPosition{0, -1},
		position + CellPosition{1, 0},
		position + CellPosition{-1, 0}
	};

	for (auto land : toTryList) {
		if (!state.board.isCaseInBoard(land))
			continue;

		auto dist = position - opponentPosition;

		if (std::abs(dist.x) < 2 && std::abs(dist.y) < 2)
			continue;

		auto diff = land - position;

		SidePosition barrier{(SideVector)position, true};
		if (diff == CellPosition{-1, 0})
			barrier = SidePosition{(SideVector)position, false};
		if (diff == CellPosition{0, 1})
			barrier = SidePosition{(SideVector)position+ SideVector{0, 1}, true};
		if (diff == CellPosition{1, 0})
			barrier = SidePosition{(SideVector)position+ SideVector{1, 0}, false};

		if (!state.board.getCell(barrier).isNone())
			continue;

		if (!state.board.getCell(land).isNone())
			continue;

		actions.push_back(BullAction{
			manager, state.player, state.step+1, CellPath{position, land}
		});
	}
}


std::vector<SidePosition> BullAction::getPawnHorizontalSpecificMoves(
	SideVector position, bool isWhite, bool initPosition
) {
	std::vector<SidePosition> lands;

	if (isWhite) {
		if (initPosition)
			lands.push_back(SidePosition{position + SideVector{0, -2}, true});

		lands.push_back(SidePosition{position + SideVector{0, -1}, true});
		lands.push_back(SidePosition{position + SideVector{0, -1}, false});
		lands.push_back(SidePosition{position + SideVector{1, -1}, false});
	} else {
		if (initPosition)
			lands.push_back(SidePosition{position + SideVector{0, 2}, true});

		lands.push_back(SidePosition{position + SideVector{0, 1}, true});
		lands.push_back(SidePosition{position, false});
		lands.push_back(SidePosition{position + SideVector{1, 0}, false});
	}

	return lands;
}

std::vector<SidePosition> BullAction::getPawnVerticalSpecificMoves(
	SideVector position, bool isWhite
) {
	std::vector<SidePosition> lands;

	if (isWhite) {
		lands.push_back(SidePosition{position + SideVector{0, -1}, false});
		lands.push_back(SidePosition{position, true});
		lands.push_back(SidePosition{position + SideVector{-1, 0}, true});
	} else {
		lands.push_back(SidePosition{position + SideVector{0, 1}, false});
		lands.push_back(SidePosition{position + SideVector{0, 1}, true});
		lands.push_back(SidePosition{position + SideVector{-1, 1}, true});
	}

	return lands;
}

/*
	We assume no capture is possible when starting this function.
*/
void BullAction::getPawnHorizontalMoves(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions
) {	
	auto *board = &state.board;
	bool isWhite = state.player == WhitePlayer;
	bool initPosition = isWhite ? (position.sideVector.y == 6) : (position.sideVector.y == 1);
	
	auto moves = getPawnHorizontalSpecificMoves(
		position.sideVector, isWhite, initPosition
	);

	for (auto move : moves)
	if (board->isCaseInBoard(move) && board->isCaseEmpty(move))
		actions.push_back(BullAction{
			manager, state.player, state.step, SidePath{position, move}
	});

}

void BullAction::getPawnVerticalMoves(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions
) {	
	auto *board = &state.board;
	bool isWhite = state.player == WhitePlayer;


	auto moves = getPawnVerticalSpecificMoves(
		position.sideVector, isWhite
	);

	for (auto move : moves)
	if (board->isCaseInBoard(move) && board->isCaseEmpty(move))
		actions.push_back(BullAction{
			manager, state.player, state.step, SidePath{position, move}
		});
}

std::vector<BullAction> BullAction::getMoves(const BullManager *manager, const BullState & state) {
	auto *board = &state.board;
	std::vector<BullAction> actions;


	// HORIZONTALS

	for (int x = 0; x < (int)board->getDimension(); x++)
	for (int y = 0; y <= (int)board->getDimension(); y++) {
		SidePosition position{SideVector{x, y}, true};

		if (board->getCell(position).isOwner(state.player)) {
			if (board->getCell(position).isPawn())
				getPawnHorizontalMoves(manager, state, position, actions);
			else
				getQueenHorizontalMoves(manager, state, position, actions);
		}
	}

	// VERTICALS

	for (int x = 0; x <= (int)board->getDimension(); x++)
	for (int y = 0; y < (int)board->getDimension(); y++) {
		SidePosition position{SideVector{x, y}, false};

		if (board->getCell(position).isOwner(state.player)) {
			if (board->getCell(position).isPawn())
				getPawnVerticalMoves(manager, state, position, actions);
			else
				getQueenVerticalMoves(manager, state, position, actions);

		}
	}

	CellPosition ourKing;
	CellPosition theirKing;

	for (int x = 0; x < (int)board->getDimension(); x++)
	for (int y = 0; y < (int)board->getDimension(); y++) {
		CellPosition position{x, y};
		if (board->getCell(position).isOwner(state.player)) {
			ourKing = position;
			getKingMoves(manager, state, position, actions);
		} else if (!board->getCell(position).isNone())
			theirKing = position;
	}

	// no king capture is allowed, need to check that
	std::vector<BullAction> actionsWithoutKingCapture;
	for (auto action : actions)
		if (
			!isThereAKingCapture(action, ourKing) &&
			!isThereAKingCapture(action, theirKing)
		) actionsWithoutKingCapture.push_back(action);

	return actionsWithoutKingCapture;
}

/*
	Pawn can only capture on y axis when they are horizontal.
*/
void BullAction::getPawnCaptures(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions
) {
	auto *board = &state.board;
	bool isWhite = state.player == WhitePlayer;
	PlayerId opponent = state.player == WhitePlayer ? 
		BlackPlayer : WhitePlayer;
		
	const SideVector dir = isWhite ? 
		SideVector{0, -1} : 
		SideVector{0, 1};
	const SideVector jump = 2*dir;

	SidePath path{position};

	auto between = SidePosition(position.sideVector + dir, true);
	auto next = SidePosition(position.sideVector + jump, true);

	while (
		board->isCaseInBoard(next) &&
		board->getCell(between).isOwner(opponent) &&
		board->getCell(next).isNone()
	) {
		path.push_back(next);
		between.sideVector += jump;
		next.sideVector += jump;
	}

	if (path.size() > 1)
		actions.push_back(BullAction{
			manager, state.player, state.step+1, path
		});
}

void BullAction::getQueenCaptures(
	const BullManager *manager, const BullState & state,
	SidePosition position, std::vector<BullAction> & actions,
	const SideVector dir
) {
	auto *board = &state.board;
	PlayerId opponent = state.player == WhitePlayer ? 
		BlackPlayer : WhitePlayer;
		
	// find range (of jumpables positions)

	SidePosition from = position;
	SidePosition to = position;
	SidePosition to_2 = position;
	to.sideVector += dir;
	to_2.sideVector += dir + dir;
	
	bool anyCapture = false;

	if (!board->isCaseInBoard(to) || board->getCell(to).isOwner(state.player))
		return;

	while (board->isCaseInBoard(to_2)) {
		auto last = to;
		to.sideVector += dir;
		to_2.sideVector += dir;

		if (board->getCell(to).isOwner(state.player))
			break;

		if (!anyCapture &&
			board->getCell(to).isNone() && 
			board->getCell(last).isOwner(opponent)) {
				anyCapture = true;
				from.sideVector = to.sideVector;
			}
	} 

	if (!anyCapture)
		return;

	SidePosition candidate = from;
	while (candidate.sideVector != to.sideVector) {
		actions.push_back(BullAction{
			manager, state.player, state.step+1, SidePath{
				position, candidate
			}
		});
		candidate.sideVector += dir;
	}
}

/*
	Captures must be maximal along axis
*/
std::vector<BullAction> BullAction::getCaptures(
	const BullManager *manager, const BullState & state
) {

	auto *board = &state.board;
	std::vector<BullAction> actions;

	// HORIZONTALS

	for (int x = 0; x < (int)board->getDimension(); x++)
	for (int y = 0; y <= (int)board->getDimension(); y++) {
		SidePosition position{SideVector{x, y}, true};

		if (board->getCell(position).isOwner(state.player)) {
			if (board->getCell(position).isPawn())
				getPawnCaptures(manager, state, position, actions);
			else {
				getQueenCaptures(manager, state, position, actions, SideVector{0, 1});
				getQueenCaptures(manager, state, position, actions, SideVector{0, -1});
			}		
		}
	}

	// VERTICALS

	for (int x = 0; x <= (int)board->getDimension(); x++)
	for (int y = 0; y < (int)board->getDimension(); y++) {
		SidePosition position{SideVector{x, y}, false};

		if (board->getCell(position).isOwner(state.player))
		if (board->getCell(position).isQueen()) {
			getQueenCaptures(manager, state, position, actions, SideVector{1, 0});
			getQueenCaptures(manager, state, position, actions, SideVector{-1, 0});
		}
	}

	CellPosition ourKing;
	CellPosition theirKing;

	for (int x = 0; x < (int)board->getDimension(); x++)
	for (int y = 0; y < (int)board->getDimension(); y++) {
		CellPosition position{x, y};
		if (board->getCell(position).isOwner(state.player))
			ourKing = position;
		else if (!board->getCell(position).isNone())
			theirKing = position;
	}

	// no king capture is allowed, need to check that
	std::vector<BullAction> actionsWithoutKingCapture;
	for (auto action : actions)
		if (
			!isThereAKingCapture(action, ourKing) &&
			!isThereAKingCapture(action, theirKing)
			)
			actionsWithoutKingCapture.push_back(action);

	return actionsWithoutKingCapture;
}


/*
	If captures are availibles, forced to capture
*/
std::vector<BullAction> BullAction::getActions(const BullManager *manager, BullState state) {
	auto captures = getCaptures(manager, state);
	
    if (captures.size() > 0)
        return captures;
    return getMoves(manager, state);
}

// -----------------------------------------------------------------------

bool BullAction::isValid(BullState state) const {
	if (this->askForPat)
		return true;

	if (this->surrend)
		return true;

	if (isSidePath) {
		if (sideJumps.size() == 0 || sideJumps.size() == 1)
			return false;

		for (auto jump : this->sideJumps)
			if (!state.board.isCaseInBoard(jump))
				return false;

		if (state.board.getCell(sideJumps[0]).owner() != this->author)
			return false;
	} else {
		if (cellJumps.size() != 2) 
			return false;
		for (auto jump : this->cellJumps)
			if (!state.board.isCaseInBoard(jump))
				return false;
		if (state.board.getCell(cellJumps[0]).owner() != this->author)
			return false;
		if (!state.board.getCell(cellJumps[0]).isKing())
			return false;
	}

	// be careful, action need to be "appliable" to use actionEquivalence
		// ie. apply function don't throw an error
	// valid => appliable but the the reciproc can be false

	auto actions = BullAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

	return false;
}

BullState BullAction::apply(BullState state) const {
	auto cells = state.board.cellPieces;
	auto sidesH = state.board.horizontalSidePieces;
	auto sidesV = state.board.verticalSidePieces;
	
	PlayerId nextPlayer = state.player == 0 ? 1 : 0;

	if (surrend || askForPat) {
		BoardSided nextBoard{
			cells, sidesH, sidesV, nextPlayer
		};

		return BullState{nextBoard, state.scores, state.step+1, nextPlayer};
	}

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
	if (surrend)
		return "Surrend";
	if (askForPat)
		return "Ask for pat";

	if (isSidePath)
    	return "[Sides] - " + Cli::toString(sideJumps);
	
	return "[Cells] - " + Cli::toString(cellJumps);
}