#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

#include <random>

// ------------------------------------------------------------
// INITIALISATION

LootManager::LootManager(const int nPlayers, const int nBots) : 
    Manager<LootAction, Board>{makePlayers(nPlayers, nBots)} 
{
    states.push_back(LootState(
        initialBoard(), 
        ScoreList(nPlayers + nBots, 0)
        ));
};

std::vector<Player> LootManager::makePlayers(
    const int nPlayers, const int nBots) 
{
    std::vector<int> outputIndices;
    for (int i = 0; i < nPlayers+nBots; i++)
        outputIndices.push_back(i);

    std::random_shuffle(outputIndices.begin(), outputIndices.end());

    std::vector<Player> output;
    for (int i = 0; i < nPlayers; i++) {
        int id = outputIndices[i];
        output.push_back(Player {
            id,
            "Player_" + std::to_string(i),
            false
        });
    }

    for (int i = nPlayers; i < nBots+nPlayers; i++) {
        int id = outputIndices[i];
        output.push_back(Player {
            id,
            "Bots_" + std::to_string(i),
            true
        });
    }

    std::vector<Player> shuffledOutput;
    for (auto i : outputIndices)
        shuffledOutput.push_back(output[i]);

    std::string players = "";
    for (auto player : shuffledOutput)
        players += " - " + player.name + "\n";
    Cli::info("Players are : \n" + players);

    return shuffledOutput;
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

bool LootManager::canPlayAction(LootState state) const {
    return LootAction::hasRemainingActions(this, state);
}