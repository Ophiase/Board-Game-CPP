#include "engine/loot/LootAction.hpp"
#include "engine/loot/LootManager.hpp"

////////////////////////////////////////////////////////////////////////
// STATIC

std::vector<CellPosition> const LootAction::authorizedOffsets = {
    {2, 0}, {0, 2}, {-2, 0}, {0, -2},
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2}
};

/*
    Given a capture/visited, we want to add to
    current captures/visiteds a all the pair such that :
        - the pair wasn't previously found
        - the pair extend the given pair 
*/
void LootAction::expandCombination(
    CellPosition initPosition,
    std::vector<CapturePath> & result,
    std::vector<CapturePath> & currentCapturePaths,
    CapturePath capturePathToExpand,
    Board board
) {
    // for every jump we try to make an outside jump
    for (auto position : capturePathToExpand.visiteds) 
    for (auto offset : LootAction::authorizedOffsets) {
        auto jump = position + offset;
        auto mid = position + offset/2;

        // is jump correct ?

        if (!board.isCaseInBoard(jump))
            continue;

        if (!board.isCaseEmpty(jump) && jump != initPosition)
            continue;

        if (board.isCaseEmpty(mid))
            continue;

        auto captureExpension = capturePathToExpand.captures;
        auto visitExpension = capturePathToExpand.visiteds;

        if (captureExpension.has(mid))
            continue;

        captureExpension.push_back(mid);
        visitExpension.push_back(jump);

        /*
            We now check that we didn't already 
            computed this combination.
        */
        bool canBeAdded = true;
        for (auto captureAlreadyComputed : currentCapturePaths)
            if (captureExpension == captureAlreadyComputed.captures) {
                canBeAdded = false;
                break;
            }
        
        if (!canBeAdded) continue;

        // add it to the current batch and result
        CapturePath expension{captureExpension, visitExpension};
        currentCapturePaths.push_back(expension);
        result.push_back(expension);
        }
}

/*
    This function find all the combinations of positions we can capture.
*/
std::vector<CapturePath> LootAction::combinationsOfCapture(
    CellPosition initPosition, Board board) {

    std::vector<CapturePath> result;

    std::vector<CapturePath> lastCapturePaths{
        CapturePath{Combination{}, Combination{initPosition}}
    };
    std::vector<CapturePath> currentCapturePaths;

    while (!lastCapturePaths.empty()) {
        for (auto capturePath : lastCapturePaths)
            expandCombination(initPosition,
                result, currentCapturePaths, capturePath, board
            );

        lastCapturePaths = currentCapturePaths;
        currentCapturePaths.clear();
    }

    return result;
}

std::vector<CellPath> LootAction::expendPaths(
    std::vector<CellPath> axioms, Board board) {

    std::vector<CellPath> result;

    for (auto axiom : axioms) {
        // Find all the combinations of pieces that can be captured
        auto capturePaths = combinationsOfCapture(axiom[0], board);
        for (auto capturePath : capturePaths)
            result.push_back(capturePath.toCellPath(board));
    }

    return result;
}

std::vector<LootAction> LootAction::getActions(
    const LootManager* manager, 
    PlayerId authorId, 
    uint step,
    Board board) { 

    /*
        You can only move of 2 on x and y axis.
        It means, that remove pieces from the board
        don't change where you can jump.
        
        It means that for every sequence of jump (starting from a yellow),
        you can always complet in such a way that you take
        the unique maximal combination of pieces

        The worsts case senario (were there are a maximal number of jumpable pieces) :
        x is an availible position, # a jumpable piece, ! not availible jump

        x#x#x#x! !x#x#x#x !!!!!!!! and an other symmetry
        #######! !####### !x#x#x#x
        x#x#x#x! !x#x#x#x !#######
        #######! !####### !x#x#x#x
        x#x#x#x! !x#x#x#x !#######
        #######! !####### !x#x#x#x
        x#x#x#x! !x#x#x#x !#######
        !!!!!!!! !!!!!!!! !x#x#x#x
        
        In the worst case, a combination has 8x3 + 4x4 = 40 elements
        Which means there are at most 2^40 combinations. 
        Fortunately, most of them are incorrects because most elements 
        require to jump on multiple elements before.
        And in practice there are very few moves availibles.
    */

    std::vector<LootAction> result;
    std::vector<CellPath> paths;
    
    // every action start by selectioning a yellow pawnCellPosition
    
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        if (board.getCell(x, y).pieceType == CellPieceType::YellowPawn)
            paths.push_back(CellPath{CellPosition{x, y}});

    if (step >= manager->players.size())
        // we expand paths only if we are not on first round
        // (on first round, you can only capture one yellow pawn) 
        paths = expendPaths(paths, board);

    for (auto path : paths)
        result.push_back(LootAction{manager, authorId, step, path});    
    
    return result;
};


/*
    Is there any authorized/correct action ?
*/
bool LootAction::hasRemainingActions(
    const LootManager *manager,
    PlayerId author,
    uint step,
    Board board) {

    (void)author;
     
    /* initialisation of the party */
    if (step < manager->players.size())
        return true;
    
    std::vector<LootAction> result;
    std::vector<CellPosition> yellows;
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        if (board.getCell(x, y).pieceType == CellPieceType::YellowPawn)
            yellows.push_back(CellPosition{x, y});

    for (auto yellow : yellows) for (auto offset : LootAction::authorizedOffsets) {
        CellPosition between = yellow + (offset)/2;
        CellPosition afterJump = yellow + offset;
        
        if (!board.isCaseInBoard(between) || !board.isCaseInBoard(afterJump))
            continue;
    
        if (!board.isCaseEmpty(between) &&
            board.isCaseEmpty(afterJump))
            return true;
    }

    return false;
};

////////////////////////////////////////////////////////////////////////
// NOT STATIC

/*
    if and only if (<=>) logic gate
*/
inline bool iff(bool const a, bool const b) {
    return (a && b) || (!a && !b);
};

bool LootAction::isValid(Board configuration) const { 
    if (jumps.size() == 0)
        return false;

    if (!iff(
        step < manager->players.size(),
        jumps.size() == 1
        )) return false;

    if (!configuration.isCaseInBoard(jumps[0]))
        return false;

    if (configuration.getCell(jumps[0]) != CellPieceType::YellowPawn)
        return false;

    for (uint i = 1; i < jumps.size(); i++) {

        CellPosition lastPosition = jumps[i-1];
        CellPosition currentPosition = jumps[i];

        CellPosition between = (lastPosition+currentPosition)/2;
        if (configuration.isCaseEmpty(between))
            return false;

        CellPosition diff = lastPosition-currentPosition;

        if (!(
            (diff == CellPosition(2, 0)) || (diff == CellPosition(-2, 0)) ||
            (diff == CellPosition(0, 2)) || (diff == CellPosition(0, -2)) ||
            (diff == CellPosition(2, 2)) || (diff == CellPosition(-2, -2)) ||
            (diff == CellPosition(-2, 2)) || (diff == CellPosition(2, -2))
        ))
            return false;

        if (!configuration.isCaseInBoard(currentPosition))
            return false;

        CellPiece current = configuration.getCell(currentPosition);
        if (!(
            current == CellPieceType::NoneCell ||
            currentPosition == jumps[0]
            ))
            return false;
    }
    
    return true;
};

void LootAction::removePointsFromScore(Board board, int & score) const {
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        switch (board.getCell(x, y).pieceType) {
            case CellPieceType::YellowPawn : 
                score -= this->manager->YELLOW_BONUS; break;
            case CellPieceType::RedPawn : 
                score -= this->manager->RED_BONUS; break;
            case CellPieceType::BlackPawn : 
                score -= this->manager->BLACK_BONUS; break;
            case CellPieceType::NoneCell :
                break;

            default : throw NotImplemented();
        }
}

std::tuple<Board, ScoreList> LootAction::apply(
    Board board, ScoreList scores) const 
{
    auto cells = board.cellPieces;
    int moveScore = this->manager->YELLOW_BONUS;

    cells[jumps[0].y][jumps[0].x] = 
        CellPiece(CellPieceType::NoneCell); 
    for (uint i = 1; i < jumps.size(); i++) {
        CellPosition lastPosition = jumps[i-1];
        CellPosition currentPosition = jumps[i];

        CellPosition between = (lastPosition+currentPosition) / 2;
        cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
        switch (board.getCell(between).pieceType) {
            case CellPieceType::YellowPawn : 
                moveScore += manager->YELLOW_BONUS; break;
            case CellPieceType::RedPawn : 
                moveScore += manager->RED_BONUS; break;
            case CellPieceType::BlackPawn : 
                moveScore += manager->BLACK_BONUS; break;

            default : throw NotImplemented();
        }
    }

    int nPlayers = manager->players.size();
    int authorIndex = manager->getPlayerIndex(author);

    PlayerId nextPlayer = (
        manager->players[(authorIndex + 1) % nPlayers]
        ).id;
    
    
    scores[authorIndex] += moveScore;
    
    
    Board nextBoard{cells, nextPlayer};
    if (manager->isFinished(nextPlayer, step+1, nextBoard)) {
        this->removePointsFromScore(nextBoard, scores[authorIndex]);   
    }
    
    return std::make_tuple(nextBoard, scores);
}