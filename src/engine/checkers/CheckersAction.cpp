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

CellPosition CheckersAction::normalizeJumpOffset(CellPosition offset) {
    int x = offset.x > 0 ? 1 : (offset.x < 0 ? -1 : 0);
    int y = offset.y > 0 ? 1 : (offset.y < 0 ? -1 : 0);
    return CellPosition{x, y};
}

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

Combination CheckersAction::toCaptured(CheckersState state) const {
    Combination captured;
    CellPosition init = jumps[0];

    if (state.board.getCell(init).isPawn()) {
        // faster implementation in pawn case 
        
        for (uint i = 1; i < jumps.size(); i++) {
            auto current = jumps[i-1];
            auto next = jumps[i];
            auto mid = (current + next) / 2;
            captured.push_back(mid);
        }

        return captured;
    } 

    for (uint i = 1; i < jumps.size(); i++) {
        auto current = jumps[i-1];
        auto next = jumps[i];

        auto offset = normalizeJumpOffset(next-current);
        current += offset;
        while (current != next) {
            if (!state.board.getCell(current).isNone())
                captured.push_back(current);
            current += offset;
        }
    }

    return captured;
};

void CheckersAction::completeSpecificPawnActions(
    const CheckersManager *manager, 
    const CheckersState & state,

    std::vector<CellPath> & visited,
    std::vector<CellPath> & nextVisited,
    CellPath currentPath
) {

    auto *board = &state.board;
    auto initPosition = currentPath[0];
    auto currentPosition = currentPath[currentPath.size()-1]; 

    Combination alreadyCaptured = CheckersAction{
        manager, state.player, state.step, currentPath}.toCaptured(state);
    
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

void CheckersAction::completeSpecificQueenActions(
    const CheckersManager *manager, 
    const CheckersState & state,

    std::vector<CellPath> & visited,
    std::vector<CellPath> & nextVisited,
    CellPath currentPath
) {

    auto *board = &state.board;
    auto initPosition = currentPath[0];
    auto currentPosition = currentPath[currentPath.size()-1]; 

    Combination alreadyCaptured = CheckersAction{
        manager, state.player, state.step, currentPath}.toCaptured(state);
    
    Cli::warning("Queen actions not implemented");
    return;
    
    throw NotImplemented();

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


std::vector<CheckersAction> CheckersAction::getSpecificActions(
    const CheckersManager *manager, const CheckersState & state, CellPosition axiom
) {
    std::vector<CellPath> visited{};
    std::vector<CellPath> nextVisited{CellPath{axiom}};

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

    } while(!nextVisited.empty());

    if (depth == 1)
        return std::vector<CheckersAction>{};
    
    std::vector<CheckersAction> actions{};

    for (auto path : visited)
        actions.push_back(CheckersAction{
            manager, state.player, state.step, path
        });

    return actions;
}


std::vector<CheckersAction> CheckersAction::getCaptures(
    const CheckersManager *manager, const CheckersState & state) 
{
    const int dimension = (int)state.board.getDimension();
    const Board *board = &state.board;
    std::vector<CheckersAction> actions{};

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
    
    return actions;
}

std::vector<CheckersAction> CheckersAction::getActions(
    const CheckersManager *manager, CheckersState state) 
{
    Cli::debug("Search actions");

    auto captures = getCaptures(manager, state);

    int maxCapture = 0;

    for (auto capture : captures)
        maxCapture = std::max(maxCapture, (int)capture.jumps.size());

    if (maxCapture > 0) { 
        // if player can capture, he's forced to capture
        
        std::vector<CheckersAction> actions{};
        for (auto capture : captures)
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

CheckersState CheckersAction::apply(
    CheckersState state) const 
{
    auto *board = &state.board;
    auto scores = state.scores;
    auto cells = board->cellPieces;

    // compute next cells

    Combination captureds = this->toCaptured(state);
    for (auto captured : captureds)
        cells[captured.y][captured.x] = CellPieceType::NoneCell;

    CellPosition first = jumps[0];
    CellPosition last = jumps[jumps.size() - 1];

    if (first != last) {
        cells[last.y][last.x] = board->getCell(first).pieceType;
        cells[first.y][first.x] = CellPieceType::NoneCell;
    }

    // misc

    int nPlayers = manager->players.size();
    int authorIndex = manager->getPlayerIndex(author);
    PlayerId nextPlayer = (
        manager->players[(authorIndex + 1) % nPlayers]
        ).id;
    
    scores[authorIndex] += captureds.size();
    
    Board nextBoard{cells, nextPlayer};
    
    CheckersState nextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
    };

    return nextState;
}

std::string CheckersAction::toString() const {
    return Cli::toString(jumps);
};