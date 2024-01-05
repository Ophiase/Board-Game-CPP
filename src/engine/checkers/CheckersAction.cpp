#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"
#include "engine/loot/Combination.hpp"

std::vector<CellPosition> const CheckersAction::allPawnOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CellPosition> const CheckersAction::directOffsets = {
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CellPosition> const CheckersAction::jumpOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
};

/*
    Cellpath need to represents a succession of jumps
    ie. more than 1 position.

    We can just check if :
        First position and last positions are equals
        and in between positions are shared.
        (it's a specific property of checkers)
*/
bool CheckersAction::equivalentCellPath(
    const CellPath & path1, const CellPath & path2 
) {
    if (path1[0] != path2[0]) 
        return false;
    if (path1[path1.size()-1] != path2[path2.size()-1])
        return false;
    if (path1.size() != path2.size())
        return false;

    // check if every positions visited by first path, 
    // is also visited by the second one
    for (auto p : path1) {
        bool has = false;
        for (auto p2 : path2)
            if ((has = (p == p2)))
                break;
        if (!has)
            return false;
    }

    return true;
}

bool CheckersAction::actionEquivalence(
    CheckersState, const CheckersAction &other) const {
    return equivalentCellPath(jumps, other.jumps);
}

std::vector<CheckersAction> CheckersAction::getPawnMoves(
    const CheckersManager *manager, const CheckersState & state) 
{
    const int dimension = (int)state.board.getDimension();
    const Board *board = &state.board;
    std::vector<CheckersAction> actions{};

    Cli::debug("looking for moves : " + std::to_string(board->player));
    Cli::debug(board->toString());

    for (int x = 0; x < dimension; x++)
    for (int y = 0; y < dimension; y++) 
    if (
        (board->getCell(x, y).owner() == board->player) &&
        board->getCell(x, y).isPawn()
    )
    for (auto offset : directOffsets) {
        CellPosition axiom{x, y};
        CellPosition jump{x+offset.x, y+offset.y};
        Cli::debug(Cli::toString(axiom) + " : " + Cli::toString(jump));

        if (board->isCaseInBoard(jump) && board->getCell(jump).isNone()) {
            actions.push_back(CheckersAction{
                manager, state.player, state.step, CellPath{
                    CellPosition{x, y}, jump 
                } });
            Cli::debug(actions[actions.size()-1].toString());
        }
    }

    Cli::debug("finished to search moves");

    return actions;
}

std::vector<CheckersAction> CheckersAction::getQueenMoves(
    const CheckersManager * manager, const CheckersState & state) 
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

void CheckersAction::completeSpecificPawnActions(
    const CheckersManager *, 
    const CheckersState & state,

    std::vector<CellPath> & visited,
    std::vector<CellPath> & nextVisited,
    CellPath currentPath
) {

    auto *board = &state.board;
    auto initPosition = currentPath[0];
    auto currentPosition = currentPath[currentPath.size()-1]; 

    Combination alreadyCaptured;
    for (uint i = 1; i < currentPath.size(); i++) {
        auto current = currentPath[i-1];
        auto next = currentPath[i];
        auto mid = (current + next) / 2;
        alreadyCaptured.push_back(mid);
    }
    
    for (auto offset : jumpOffsets) {
        auto toPosition = currentPosition + offset;
        auto between = currentPosition + (offset / 2);

        if (!board->isCaseInBoard(toPosition))
            continue;

        if ( (toPosition != initPosition) && 
            !board->isCaseEmpty(toPosition) )
            continue;
        
        if (
            board->isCaseEmpty(between) || alreadyCaptured.has(between))
            continue;

        if (board->getCell(between).owner() == state.player)
            continue;


        CellPath next = currentPath;
        next.push_back(toPosition);

        bool redundant = false;
        for (auto cmp : visited)
            if ((redundant = equivalentCellPath(cmp, next)))
                break;
        if (redundant) break;

        nextVisited.push_back(next);
    }
}

std::vector<CheckersAction> CheckersAction::getSpecificPawnActions(
    const CheckersManager *manager, const CheckersState & state, CellPosition axiom
) {
    Cli::debug("\tSearch on " + Cli::toString(axiom));

    std::vector<CellPath> visited{};
    std::vector<CellPath> nextVisited{CellPath{axiom}};

    int depth = 0;

    do {
        depth++;
        visited = nextVisited;
        nextVisited.clear();

        for (auto path : visited)
            completeSpecificPawnActions(
                manager, state, 
                
                visited,
                nextVisited,
                path
            );
    } while(!nextVisited.empty());

    if (depth == 1)
        return std::vector<CheckersAction>{};
    
    Cli::debug("\t\t Found !");

    std::vector<CheckersAction> actions{};
    for (auto path : visited)
        actions.push_back(CheckersAction{
            manager, state.player, state.step, path
        });

    return actions;
}

std::vector<CheckersAction> CheckersAction::getPawnCaptures(
    const CheckersManager *manager, const CheckersState & state) 
{
    const int dimension = (int)state.board.getDimension();
    const Board *board = &state.board;
    std::vector<CheckersAction> actions{};

    for (int x = 0; x < dimension; x++)
    for (int y = 0; y < dimension; y++)
    if (
        (board->getCell(x, y).owner() == board->player) &&
        board->getCell(x, y).isPawn()
    ) {
        auto actionsToAdd = getSpecificPawnActions(
            manager,
            state,
            CellPosition{x, y});
        for (auto action : actionsToAdd)
            actions.push_back(action);
    }
    
    return actions;
}

std::vector<CheckersAction> CheckersAction::getQueenCaptures(
    const CheckersManager *manager, const CheckersState & state) 
{
    (void)manager;
    (void)state;

    Cli::warning("Queen action not supported yet!");
    return std::vector<CheckersAction>{};

    throw NotImplemented();
}

std::vector<CheckersAction> CheckersAction::getActions(
    const CheckersManager *manager, CheckersState state) 
{
    Cli::debug("Search actions");

    auto pawnCaptures = getPawnCaptures(manager, state);
    auto queenCaptures = getQueenCaptures(manager, state);

    Cli::debug("Looking at found captures");

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

    Cli::debug("No Captures");

    auto moves = getPawnMoves(manager, state);
    auto queenMoves = getQueenMoves(manager, state);
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

    for (auto pawn : pawns) for (auto offset : CheckersAction::allPawnOffsets) {
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
bool CheckersAction::isValidPawnMove(const CheckersState & state) const {
    Cli::debug("check pawn action : " + this->toString());
    auto actions = CheckersAction::getActions(this->manager, state);
    for (auto action : actions) {
        Cli::debug("\tvs : " + action.toString());
        if (this->actionEquivalence(state, action))
            return true;
    }
    Cli::debug("not checked");
    return false;
}

// TODO OPTIMIZE
bool CheckersAction::isValidQueenMove(const CheckersState & state) const {
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
        return isValidQueenMove(state);

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