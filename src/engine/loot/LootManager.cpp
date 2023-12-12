#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

#include <random>

// ------------------------------------------------------------
// INITIALISATION

LootManager::LootManager(int nPlayers, int nBots) : 
    Manager<LootAction, Board>{makePlayers(nPlayers, nBots)} {
    configurations.push_back(initialBoard());
    scores.push_back(scoreList(nPlayers + nBots, 0));
};

std::vector<Player> LootManager::makePlayers(
    int nPlayers, int nBots) {
    std::vector<Player> output;

    for (int i = 0; i < nPlayers; i++)
        output.push_back(Player {
            i,
            "Player_" + std::to_string(i),
            false
        });

    for (int i = 0; i < nBots; i++)
        output.push_back(Player {
            i,
            "Bots_" + std::to_string(i),
            true
        });

    // shuffle ?

    return output;
}

CellPiece LootManager::randomCellPiece(int & ry, int & rd, int & rb) {
    srand(time(NULL));
    int r = rand() % (ry+rd+rb);

    if (r < ry) {
        ry--;
        return CellPiece(CellPieceType::YellowPawn);
    }

    if (r < ry+rd) {
        rd--;
        return CellPiece(CellPieceType::RedPawn);
    }

    rb--;
    return CellPiece(CellPieceType::BlackPawn);
};

Board LootManager::initialBoard() {
    std::vector<std::vector<CellPiece>> cellPieces{
        8, std::vector<CellPiece>(8)
    };

    int remaningYellow = TOTAL_YELLOW_PAWN;
    int remaningRed = TOTAL_RED_PAWN;
    int remaningBlack = TOTAL_BLACK_PAWN;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            cellPieces[i][j] = 
            randomCellPiece(remaningYellow, remaningRed, remaningBlack);

    return Board(cellPieces, 0);
};

// ------------------------------------------------------------

std::vector<Combination> LootManager::combinationsOfCapture(CellPosition, Board) const {
    throw NotImplemented();

    std::vector<Combination> result;

    std::vector<Combination> lastBatch;
    std::vector<Combination> currentBatch;

    // first iteration
        // evaluates the 8 first jumps
        // put corrects one them in lastBatch and result 

    while (!lastBatch.empty()) {
        for (auto combination : lastBatch) {
            // find all the positions we can start from

            // for every position, tries to find a new jump

            // check if we don't add two time to currentBatch a same combination

            // add it to the current batch and result
        }

        lastBatch = currentBatch;
        currentBatch.clear();
    }

    return result;
}

std::vector<CellPath> LootManager::expendPaths(std::vector<CellPath> axioms, Board) const {
    throw NotImplemented();

    for (auto axiom : axioms) {
        // Find all the combinations of pieces that can be captured
    }
}

std::vector<LootAction> LootManager::getActions(Board board) const {
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
    
    throw NotImplemented();

    PlayerId authorId = this->getCurrentPlayer().id;
    std::vector<LootAction> result;
    std::vector<CellPath> paths;
    
    // every action start by selectioning a yellow pawn

    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        if (board.getCell(x, y).pieceType == CellPieceType::YellowPawn)
            paths.push_back(CellPath{CellPosition{x, y}});

    if ((uint)this->step() >= this->players.size())
        // we expand paths only if we are not on first round
        // (on first round, you can only capture one yellow pawn) 
        paths = this->expendPaths(paths, board);

    for (auto path : paths)
        result.push_back(LootAction{authorId, path});    
    
    return result;
};

bool LootManager::canPlayAction(Board board) const {
    /* initialisation of the party */
    if ((uint)this->step() < this->players.size())
        return true;
    
    std::vector<LootAction> result;
    std::vector<CellPosition> yellows;
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        if (board.getCell(x, y).pieceType == CellPieceType::YellowPawn)
            yellows.push_back(CellPosition{x, y});
    
    std::vector<CellPosition> offsets = {
        {2, 0}, {0, 2}, {-2, 0}, {0, -2},
        {2, 2}, {2, -2}, {-2, 2}, {-2, -2}
    };

    for (auto yellow : yellows) for (auto offset : offsets) {
        CellPosition between = yellow + (offset)/2;
        CellPosition afterJump = yellow + offset;
        
        if (!board.isCaseInBoard(between) || !board.isCaseInBoard(afterJump))
            continue;
    
        if (!board.getCell(between).isNone() &&
            board.getCell(afterJump).isNone())
            return true;
    }

    return false;
}

/*
    if and only if (<=>) logic gate
*/
inline bool iff(bool const a, bool const b) {
    return (a && b) || (!a && !b);
};

bool LootManager::canPlayAction(LootAction action) const {
    if (action.jumps.size() == 0)
        return false;

    if (!iff(
        (uint)step() < players.size(),
        action.jumps.size() == 1
        )) return false;
    
    auto configuration = getConfiguration();
    if (!configuration.isCaseInBoard(action.jumps[0]))
        return false;
    if (configuration.getCell(action.jumps[0]) != CellPieceType::YellowPawn)
        return false;

    for (uint i = 1; i < action.jumps.size(); i++) {
        CellPosition lastPosition = action.jumps[i-1];
        CellPosition currentPosition = action.jumps[i];

        CellPosition between = (lastPosition+currentPosition)/2;
        if (configuration.getCell(between).isNone())
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
            currentPosition == action.jumps[0]
            ))
            return false;
    }
        
    return true;
};

void LootManager::removePointsFromScore(Board board, int & score) const {
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        switch (board.getCell(x, y).pieceType) {
            case CellPieceType::YellowPawn : 
                score -= YELLOW_BONUS; break;
            case CellPieceType::RedPawn : 
                score -= RED_BONUS; break;
            case CellPieceType::BlackPawn : 
                score -= BLACK_BONUS; break;
            case CellPieceType::NoneCell :
                break;

            default : throw NotImplemented();
        }
}

std::tuple<Board, scoreList> LootManager::evaluateAction(
    LootAction action, Board board) const {

    auto cells = board.cellPieces;
    int moveScore = YELLOW_BONUS;

    cells[action.jumps[0].y][action.jumps[0].x] = 
        CellPiece(CellPieceType::NoneCell); 
    for (uint i = 1; i < action.jumps.size(); i++) {
        CellPosition lastPosition = action.jumps[i-1];
        CellPosition currentPosition = action.jumps[i];

        CellPosition between = (lastPosition+currentPosition) / 2;
        cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
        switch (board.getCell(between).pieceType) {
            case CellPieceType::YellowPawn : 
                moveScore += YELLOW_BONUS; break;
            case CellPieceType::RedPawn : 
                moveScore += RED_BONUS; break;
            case CellPieceType::BlackPawn : 
                moveScore += BLACK_BONUS; break;

            default : throw NotImplemented();
        }
    }

    int nPlayers = players.size();
    int currentPlayerIndex = getCurrentPlayerIndex();
    PlayerId nextPlayer = (
        players[(currentPlayerIndex + 1) % nPlayers]
        ).id;
    
    scoreList scores{this->getScores()};
    scores[currentPlayerIndex] += moveScore;

    Board nextBoard{cells, nextPlayer};
    if (isFinished(nextBoard)) {
        removePointsFromScore(nextBoard, scores[currentPlayerIndex]);   
    }

    return std::make_tuple(nextBoard, scores);
}
