#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"

std::vector<CellPosition> const CheckersAction::authorizedOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CheckersAction> CheckersAction::getActions(
    const CheckersManager *manager, CheckersState state) 
{
    (void)manager;
    (void)state;

    throw NotImplemented();
}

/*
    Is there any authorized/correct action ?
*/
bool CheckersAction::hasRemainingActions(
    const CheckersManager *manager, CheckersState state) {

	CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    //CellPieceType rivalPawn = state.player == 0 ? CellPieceType::BlackPawn : CellPieceType::WhitePawn;
     
    /* initialisation of the party */
    if (state.step < manager->players.size())
        return true;
    
    std::vector<CheckersAction> result;
    std::vector<CellPosition> pawns;
    for (int x = 0; x < 10; x++) for (int y = 0; y < 10; y++)
        if (state.board.getCell(x, y).pieceType == playerPawn)
            pawns.push_back(CellPosition{x, y});

    for (auto pawn : pawns) for (auto offset : CheckersAction::authorizedOffsets) {
		if (offset.x == 2 || offset.y == -2) {
			CellPosition between = pawn + (offset)/2;
			CellPosition afterJump = pawn + offset;
			
			if (!state.board.isCaseInBoard(between) || !state.board.isCaseInBoard(afterJump))
				continue;
		
			if (!(state.board.isCaseEmpty(between) || state.board.getCell(between).pieceType == playerPawn) &&
				state.board.isCaseEmpty(afterJump))
				return true;
		} else if (offset.x == 1 || offset.y == -1) {
			CellPosition afterJump = pawn + offset;
			if (!state.board.isCaseInBoard(afterJump))
				continue;
			if (state.board.isCaseEmpty(afterJump))
				return true;
		}
    }

    return false;
};

inline bool iff(bool const a, bool const b) {
    return (a && b) || (!a && !b);
};

// TODO OPTIMIZE
bool CheckersAction::isValidPawnMove(CheckersState state) const {
    auto actions = CheckersAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

    return false;
}

// TODO OPTIMIZE
bool CheckersAction::isValidQueenMove(CheckersState state) const {
    auto actions = CheckersAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

    return false;
}

bool CheckersAction::isValid(CheckersState state) const { 
	if (jumps.size() == 0 || jumps.size() == 1) 
        return false;

    if (!state.board.isCaseInBoard(jumps[0]))
        return false;

    if (state.board.getCell(jumps[0]).isPawn())
        return isValidPawnMove(state);

    if (state.board.getCell(jumps[0]).isQueen())
        return isValidPawnMove(state);

    return false;
}

void CheckersAction::removePointsFromScore(Board board, int & score) const {
    for (int x = 0; x < 10; x++) for (int y = 0; y < 10; y++)
        switch (board.getCell(x, y).pieceType) {
            case CellPieceType::WhitePawn : 
                score -= this->manager->WHITE_BONUS; break;
            case CellPieceType::BlackPawn : 
                score -= this->manager->BLACK_BONUS; break;
            case CellPieceType::NoneCell :
                break;
            default : throw NotImplemented();
        }
}

CheckersState CheckersAction::apply(
    CheckersState state) const 
{
    auto board = state.board;
    auto scores = state.scores;
    auto cells = board.cellPieces;

    int moveScore = 0;
    CellPieceType playerPawn = state.player == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    
    cells[jumps[0].y][jumps[0].x] = CellPiece(CellPieceType::NoneCell);
    if (jumps.size() == 2) {
        CellPosition lastPosition = jumps[0];
        CellPosition currentPosition = jumps[1];
        CellPosition offset = currentPosition - lastPosition;
        if (offset == CellPosition(1,1) || offset == CellPosition(1,-1) || offset == CellPosition(-1,1) || offset == CellPosition(-1,-1)) {
            cells[currentPosition.y][currentPosition.x] = CellPiece(playerPawn);
            moveScore += 1;
        } else if (offset == CellPosition(2,2) || offset == CellPosition(2,-2) || offset == CellPosition(-2,2) || offset == CellPosition(-2,-2)) {
            CellPosition between = (lastPosition+currentPosition) / 2;
            cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
            cells[currentPosition.y][currentPosition.x] = CellPiece(playerPawn);
            moveScore += 1;
        }
    } else {
        for (long unsigned int i = 0 ; i < jumps.size() - 1 ; i++) {
            CellPosition lastPosition = jumps[i];
            CellPosition currentPosition = jumps[i+1];
            CellPosition offset = currentPosition - lastPosition;
            CellPosition between = lastPosition + (offset)/2;
            cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
            cells[currentPosition.y][currentPosition.x] = CellPiece(playerPawn);
            moveScore += 1;
        }
    }

    int nPlayers = manager->players.size();
    int authorIndex = manager->getPlayerIndex(author);

    PlayerId nextPlayer = (
        manager->players[(authorIndex + 1) % nPlayers]
        ).id;
    
    scores[authorIndex] += moveScore;
    
    Board nextBoard{cells, nextPlayer};
    
    CheckersState tempNextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
        };

    if (manager->isFinished(tempNextState))
        this->removePointsFromScore(nextBoard, scores[authorIndex]);

    CheckersState nextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
    };

    return nextState;
}

std::string CheckersAction::toString() const {
    return Cli::toString(jumps);
};