#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"
#include "engine/Combination.hpp"

std::vector<CellPosition> const CheckersAction::allPawnOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CellPosition> const CheckersAction::directPawnOffsets = {
	{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};

std::vector<CellPosition> const CheckersAction::jumpPawnOffsets = {
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2},
};

std::vector<CellPosition> const CheckersAction::directQueenOffsets = {
	{0, 1}, {1, 0}, {0, -1}, {-1, 0}, 
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
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
    CheckersState, const CheckersAction &other) const 
{
    if (this->surrend && other.surrend) return true;

    return equivalentCellPath(jumps, other.jumps);
}

std::vector<CheckersAction> CheckersAction::getPawnMoves(
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
    )
    for (auto offset : directPawnOffsets) {
        CellPosition axiom{x, y};
        CellPosition jump{x+offset.x, y+offset.y};
        if (board->isCaseInBoard(jump) && board->getCell(jump).isNone()) {
            actions.push_back(CheckersAction{
                manager, state.player, state.step, CellPath{
                    CellPosition{x, y}, jump 
                } });
        }
    }

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
    for (auto offset : directQueenOffsets) {
        CellPosition from{x, y};
        CellPosition to = from + offset;
        while (board->isCaseInBoard(to) && board->getCell(to).isNone()) {
            actions.push_back(CheckersAction{
                manager, state.player, state.step, CellPath{from, to} });
            to += offset;
        }
    }

    return actions;
}

Combination CheckersAction::toCaptured(CheckersState state) const {
    Combination captured;
    CellPosition init = jumps[0];

    if (state.board.getCell(init).isPawn()) {
        // faster implementation in pawn case 

        if (jumps.size() == 2 && (
                (std::abs(jumps[0].x - jumps[1].x) == 1) || 
                (std::abs(jumps[0].y - jumps[1].y) == 1)
        ))
            return captured;

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
    
    for (auto offset : jumpPawnOffsets) {
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

    for (auto offset : directQueenOffsets) {
        auto toPosition = currentPosition + offset;
        
        Combination capturing{};
        std::vector<CellPath> candidates{};

        while (board->isCaseInBoard(toPosition+offset)) {
            auto between = toPosition;
            toPosition += offset;
            if (between == initPosition)
                continue;

            if (!state.board.isCaseEmpty(between)) {
                if (state.board.getCell(between).owner() == state.player)
                    break;
                else {
                    capturing.push_back(between);
                    candidates.clear(); // forced to maximise along axis
                }
            }

            if (!state.board.isCaseEmpty(toPosition))
                continue;

            if (capturing.empty())
                continue;

            CellPath next = currentPath;
            next.push_back(toPosition);

            bool redundant = false;
            for (auto cmp : visited)
                if ((redundant = equivalentCellPath(cmp, next)))
                    break;
            if (redundant) break;

            if (candidates.size() < (uint)manager->MAX_QUEEN_BRANCHING)
                candidates.push_back(next);
        }

        for (auto candidate : candidates)
            nextVisited.push_back(candidate);       
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
    } while(!nextVisited.empty() && (isPawn || (depth <= manager->MAX_QUEEN_DEPTH)));

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
    auto captures = getCaptures(manager, state);

    int maxCapture = 0;

    for (auto capture : captures)
        maxCapture = std::max(maxCapture, (int)capture.jumps.size());

    if (maxCapture > 0) { 
        // if player can capture, he's forced to capture
        std::vector<CheckersAction> actions{};
        for (auto capture : captures)
            if (
                ((int)capture.jumps.size() == maxCapture)
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

bool CheckersAction::hasRemainingActions(
    const CheckersManager *manager, CheckersState state) 
{
    return !CheckersAction::getActions(manager, state).empty();
}

/*
    Check if a sequence of jumps inside the board, 
    starting with a queen is a valid capture.
*/
bool CheckersAction::isValidQueenCapture(const CheckersState & state) const {
    const auto *board = &state.board;
    for (uint i = 1; i < jumps.size(); i++) {
        auto current = jumps[i-1]; 
        auto next = jumps[i];

        if (!board->isCaseEmpty(next) && (next != jumps[0]))
            return false;

        auto dir = normalizeJumpOffset(next-current);

        // check there are no own piece capture 
        auto position = current+dir;
        bool haveCapture = false;
        while (position != next) {
            auto between = position;
            position += dir;

            if (board->getCell(between).owner() == this->author)
                return false;
            if (!board->getCell(between).isNone())
                haveCapture = true;
        }

        if (!haveCapture)
            return false;

        // check if it took all the piece it can in the direction
        bool potential_capture = false;
        while (board->isCaseInBoard(position+dir)) {
            position += dir;
            if (board->getCell(position).owner() == this->author)
                break;

            if (board->getCell(position).isNone()) {
                if (potential_capture)
                    return false;
                else
                    continue;
            }
            
            if (board->getCell(position).owner() != this->author)
                potential_capture = true;
        }
    }

    return true;
}

bool CheckersAction::isValidQueenAction(const CheckersState & state) const {
    /* 
        The number of queen's capture is very high. So getActions limit the
        number of queen's capture computed. We need to verify
        separatly that a queen's capture is valid.
    */

    if (isValidQueenCapture(state))
        return true;

    // We then verify if it's a queen's move.
    auto actions = CheckersAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

    return false;
}

bool CheckersAction::isValidPawnAction(const CheckersState & state) const {
    auto actions = CheckersAction::getActions(this->manager, state);
    for (auto action : actions)
        if (this->actionEquivalence(state, action))
            return true;

    return false;
}

bool CheckersAction::isValid(CheckersState state) const { 
    if (this->surrend) {
        return true;
    }

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

CheckersState CheckersAction::apply(
    CheckersState state) const 
{
    auto *board = &state.board;
    auto scores = state.scores;
    auto cells = board->cellPieces;

    int bonus = 0;

    // compute next cells

    if (!this->surrend) {
        Combination captureds = this->toCaptured(state);
        for (auto captured : captureds)
            cells[captured.y][captured.x] = CellPieceType::NoneCell;

        CellPosition first = jumps[0];
        CellPosition last = jumps[jumps.size() - 1];

        if (first != last) {
            cells[last.y][last.x] = board->getCell(first).pieceType;
            cells[first.y][first.x] = CellPieceType::NoneCell;
            
            if (this->author == WhitePlayer) {
                if (last.y == 0)
                    cells[last.y][last.x] = CellPieceType::WhiteQueen;
            } else 
                if (last.y == ((int)board->getDimension() - 1))
                    cells[last.y][last.x] = CellPieceType::BlackQueen;
        }

        bonus = captureds.size();
    }

    // misc

    int nPlayers = manager->players.size();
    int authorIndex = manager->getPlayerIndex(author);
    PlayerId nextPlayer = (
        manager->players[(authorIndex + 1) % nPlayers]
        ).id;
    
    scores[authorIndex] += bonus;
    
    Board nextBoard{cells, nextPlayer};
    
    CheckersState nextState{
        nextBoard, scores, 
        state.step+1, nextBoard.player
    };

    return nextState;
}

std::string CheckersAction::toString() const {
    if (this->surrend)
        return "[Surrend]";
    return Cli::toString(jumps);
};