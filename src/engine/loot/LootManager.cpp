#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

#include <random>

// ------------------------------------------------------------
// INITIALISATION

LootManager::LootManager(int nPlayers) : 
    Manager<LootAction, Board>{makePlayers(nPlayers)} {
    configurations.push_back(initialBoard());
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


std::vector<LootAction> LootManager::getActions() const {
    throw NotImplemented();
};

bool LootManager::canPlayAction(LootAction action) const {
    if (action.jumps.size() == 0)
        return false;
    
    auto configuration = getConfiguration();
    if (!configuration.isCaseInBoard(action.jumps[0]))
        return false;
    if (configuration.getCell(action.jumps[0]) != CellPieceType::YellowPawn)
        return false;

    for (uint i = 1; i < action.jumps.size(); i++) {
        CellPosition lastPosition = action.jumps[i-1];
        CellPosition currentPosition = action.jumps[i];

        CellPosition diff = lastPosition-currentPosition;

        if (!(
            (diff == CellPosition(2, 0)) || (diff == CellPosition(-2, 0)) ||
            (diff == CellPosition(0, 2)) || (diff == CellPosition(0, -2)) ||
            (diff == CellPosition(1, 1)) || (diff == CellPosition(-1, -1)) ||
            (diff == CellPosition(-1, 1)) || (diff == CellPosition(1, -1))
        ))
            return false;

        if (!configuration.isCaseInBoard(currentPosition))
            return false;

        CellPiece current = configuration.getCell(currentPosition);
        if (current != CellPieceType::NoneCell)
            return false;
    }
        
    return true;
};

Board LootManager::evaluateAction(
    LootAction action, Board board) const {

    auto cells = board.cellPieces;

    for (uint i = 1; i < action.jumps.size(); i++) {
        CellPosition lastPosition = action.jumps[i-1];
        CellPosition currentPosition = action.jumps[i];

        CellPosition between = (lastPosition+currentPosition) / 2;
        cells[between.y][between.x] = CellPiece(CellPieceType::NoneCell);
    }

    int nPlayers = players.size();
    PlayerId nextPlayer = (
        players[getCurrentPlayerIndex() % nPlayers]
        ).id;
    
    return Board{cells, nextPlayer};
}
