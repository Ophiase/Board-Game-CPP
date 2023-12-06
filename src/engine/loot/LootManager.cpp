#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

#include <random>

// ------------------------------------------------------------
// INITIALISATION

LootManager::LootManager(int nPlayers) : 
    Manager<LootAction, Board>{makePlayers(nPlayers)} {
    configurations.push_back(initialBoard());
    scores.push_back(scoreList(nPlayers, 0));
};

std::vector<Player> LootManager::makePlayers(int n) {
    std::vector<Player> output;

    for (int i = 0; i < n; i++)
        output.push_back(Player{
            i,
            "Player_" + std::to_string(i)
        });

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



std::vector<LootAction> LootManager::getActions(Board board) const {
    throw NotImplemented();

    PlayerId authorId = this->getCurrentPlayer().id;
    std::vector<LootAction> result;
    std::vector<CellPosition> yellows;
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
        if (board.getCell(x, y).pieceType == CellPieceType::YellowPawn)
            yellows.push_back(CellPosition{x, y});

    if ((uint)this->step() < this->players.size()) {
        for (auto yellow : yellows)
            result.push_back(LootAction{
                authorId, std::vector<CellPosition>{yellow}});
        return result;
    }

    // TODO: find all actions
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
