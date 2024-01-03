#include "engine/checkers/CheckersManager.hpp"
#include "utils/NotImplemented.hpp"

CheckersManager::CheckersManager(const int nPlayers, const int nBots) : 
    Manager<CheckersAction, Board>{makePlayers(nPlayers, nBots)} 
{
    states.push_back(CheckersState(
        initialBoard(), 
        ScoreList(nPlayers + nBots, 0)
        ));
};

std::vector<Player> CheckersManager::makePlayers(const int nPlayers, const int nBots) 
{
    if (!((nPlayers == 2 && nBots == 0) || (nPlayers == 1 && nBots == 1)))
        throw NotImplemented("Only configurations with 2 players or 1 player and 1 bot are supported");

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


Board CheckersManager::initialBoard() {
    std::vector<std::vector<CellPiece>> cellPieces{
        10, std::vector<CellPiece>(10)
    };

    for (int i = 0; i < 4; i++){
		for (int j = 0; j < 10; j++) {
			if (i % 2 == 0 && j % 2 == 1) {
				cellPieces[i][j] = CellPiece(CellPieceType::BlackPawn);
			}
			else if (i % 2 == 1 && j % 2 == 0) {
				cellPieces[i][j] = CellPiece(CellPieceType::BlackPawn);
			}
		}
	}

	for (int i = 6; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (i % 2 == 0 && j % 2 == 1) {
				cellPieces[i][j] = CellPiece(CellPieceType::WhitePawn);
			}
			else if (i % 2 == 1 && j % 2 == 0) {
				cellPieces[i][j] = CellPiece(CellPieceType::WhitePawn);
			}
		}
	}

    return Board(cellPieces, 0);
};

bool CheckersManager::canPlayAction(CheckersState state) const {
    return CheckersAction::hasRemainingActions(this, state);
}
