#include "engine/bulltricker/BullManager.hpp"
#include "utils/NotImplemented.hpp"

BullManager::BullManager(bool againstBot = false) : 
    Manager<BullAction, Board>{makePlayers(againstBot)} 
{
    states.push_back(BullState(
        initialBoard(), 
        ScoreList(2, 0)
        ));
};

std::vector<Player> BullManager::makePlayers(bool againstBot) 
{
	int nPlayers = againstBot ? 1 : 2;
	int nBots = againstBot ? 1 : 0;

    std::vector<Player> output;
    std::vector<int> outputIndices = {0, 1};
    std::random_shuffle(outputIndices.begin(), outputIndices.end());

    for (int i = 0; i < 2; i++) {
        int id = outputIndices[i];
        std::string playerName = (i < nPlayers) ? "Player_" : "Bot_";
        bool isBot = (i >= nPlayers);

        output.push_back( Player {id, playerName + std::to_string(i), isBot});
    }

    std::vector<Player> shuffledOutput;
    for (auto i : outputIndices)
        shuffledOutput.push_back(output[i]);

    std::string players;
    for (const auto& player : shuffledOutput)
        players += " - " + player.name + "\n";

    Cli::info("Players are: \n" + players);

    return shuffledOutput;
}

Board BullManager::initialBoard() {
    std::vector<std::vector<CellPiece>> cellPieces{
        7, std::vector<CellPiece>(7)
    };
    std::vector<std::vector<SidePiece>> horizontalSidePieces{
        7, std::vector<SidePiece>(8)
    };
    std::vector<std::vector<SidePiece>> verticalSidePieces{
        8, std::vector<SidePiece>(7)
    };

    cellPieces[3][0] = CellPiece(CellPieceType::BlackKing);
    cellPieces[3][6] = CellPiece(CellPieceType::WhiteKing);

    for (int i = 0; i < 7; i++) {
        horizontalSidePieces[0][i] = SidePiece(SidePieceType::BlackSidePawn);
        horizontalSidePieces[6][i] = SidePiece(SidePieceType::WhiteSidePawn);
    }
    return BoardSided(7);
};

bool BullManager::canPlayAction(BullState state) const {
    return BullAction::hasRemainingActions(this, state);
}