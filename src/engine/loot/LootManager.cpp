#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

#include <random>

// ------------------------------------------------------------
// INITIALISATION

LootManager::LootManager(int nPlayers, int nBots) : 
    Manager<LootAction, Board>{makePlayers(nPlayers, nBots)} {
    configurations.push_back(initialBoard());
    scores.push_back(ScoreList(nPlayers + nBots, 0));
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

bool LootManager::canPlayAction(PlayerId author, uint step, Board board) const {
    return LootAction::hasRemainingActions(this, author, step, board);
}