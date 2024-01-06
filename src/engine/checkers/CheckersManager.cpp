#include "engine/checkers/CheckersManager.hpp"
#include "utils/NotImplemented.hpp"

CheckersManager::CheckersManager(bool againstBot) : 
    Manager<CheckersAction, Board>{makePlayers(againstBot)} 
{
    states.push_back(CheckersState(
        initialBoard(), 
        ScoreList(2, 0)
        ));
};

std::vector<Player> CheckersManager::makePlayers(bool againstBot) 
{
    srand(time(NULL));
    int whichBot = rand() % 2;

    std::vector<Player> players{
        Player{
            WhitePlayer,
            (againstBot && whichBot == 0) ?
            "White Bot" : "White Player",
            (againstBot && whichBot == 0)
        },
        Player{
            BlackPlayer,
            (againstBot && whichBot == 1) ?
            "Black Bot" : "Black Player",
            (againstBot && whichBot == 1)
        }
    };

    std::string playersStr;
    for (const auto& player : players)
        playersStr += " - " + player.name + "\n";

    Cli::info("Players are: \n" + playersStr);

    return players;
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
    if (state == this->getState())
        if (this->getLastAction().surrend)
            return false;

    return CheckersAction::hasRemainingActions(this, state);
}
