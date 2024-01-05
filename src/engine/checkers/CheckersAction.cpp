#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"

std::vector<CellPosition> const CheckersAction::authorizedOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CellPosition> const CheckersAction::directOffsets = {
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CellPosition> const CheckersAction::jumpOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
};

std::vector<CheckersAction> CheckersAction::getPawnMoves(
    const CheckersManager * manager, CheckersState state) 
{
    const int dimension = (int)state.board.getDimension();
    const Board *board = &state.board;
    std::vector<CheckersAction> actions{};

    for (int x = 0; x < dimension; x++)
    for (int y = 0; y < dimension; y++)
    if (
        board->getCell(x, y).owner() == board->player &&
        board->getCell(x, y).isPawn()
    )
    for (auto offset : directOffsets) {
        CellPosition jump{x+offset.x, y+offset.y};

        if (board->isCaseInBoard(jump) && board->getCell(jump).isNone())
            actions.push_back(CheckersAction{
                manager, state.player, state.step, CellPath{
                    CellPosition{x, y}, jump 
                } });
    }

    return actions;
}

std::vector<CheckersAction> CheckersAction::getQueenMoves(
    const CheckersManager * manager, CheckersState state) 
{
    const int dimension = (int)state.board.getDimension();
    const Board *board = &state.board;
    std::vector<CheckersAction> actions{};

    for (int x = 0; x < dimension; x++)
    for (int y = 0; y < dimension; y++)
    if (
        board->getCell(x, y).owner() == board->player &&
        board->getCell(x, y).isQueen()
    )
    for (int x2 = 0; x2 < dimension; x2++)
    for (int y2 = 0; y2 < dimension; y2++) 
    if (
        (x = x2) || (y == y2) ||
        (std::abs(x-x2)) == (std::abs(y-y2))
    ) {
        CellPosition jump{x2, y2};
        if (board->isCaseInBoard(jump) && board->getCell(jump).isNone())
            actions.push_back(CheckersAction{
                manager, state.player, state.step, CellPath{
                    CellPosition{x, y}, jump 
                } });
    }

    return actions;
}

std::vector<CheckersAction> CheckersAction::getPawnCaptures(
    const CheckersManager * manager, CheckersState state) 
{
    (void)manager;
    (void)state;

    throw NotImplemented();
}

std::vector<CheckersAction> CheckersAction::getQueenCaptures(
    const CheckersManager * manager, CheckersState state) 
{
    (void)manager;
    (void)state;

    throw NotImplemented();
}

std::vector<CheckersAction> CheckersAction::getActions(
    const CheckersManager *manager, CheckersState state) 
{
    auto pawnCaptures = getPawnCaptures(manager, state);
    auto queenCaptures = getQueenCaptures(manager, state);

    int maxCapture = 0;

    for (auto capture : pawnCaptures)
        maxCapture = std::max(maxCapture, (int)capture.jumps.size());
    for (auto capture : queenCaptures)
        maxCapture = std::max(maxCapture, (int)capture.jumps.size());

    if (maxCapture > 0) { 
        // if player can capture, he's forced to capture
        
        std::vector<CheckersAction> actions{};
        for (auto capture : pawnCaptures)
            if ((int)capture.jumps.size() == maxCapture)
                actions.push_back(capture);
        for (auto capture : queenCaptures)
            if ((int)capture.jumps.size() == maxCapture)
                actions.push_back(capture);
        
        return actions;
    }

    auto moves = getPawnCaptures(manager, state);
    auto queenMoves = getQueenCaptures(manager, state);
    for (auto queenMove : queenMoves)
        moves.push_back(queenMove);

    return moves;
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