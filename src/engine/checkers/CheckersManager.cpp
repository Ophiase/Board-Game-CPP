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

    for (int y = 0; y < 10; y++)
    for (int x = 0; x < 10; x++) {
        if (((x+y) & 1) == 1) {
            if (y < 4)
                cellPieces[y][x] = CellPiece(CellPieceType::BlackPawn);
            else if (y >= 6)
                cellPieces[y][x] = CellPiece(CellPieceType::WhitePawn);
        }
    }

    return Board(cellPieces, 0);
};

bool CheckersManager::canPlayAction(CheckersState state) const {
    if (state == this->getState()) {
        if (this->step() > 0 && this->getLastAction().surrend)
            return false;
    }

    return CheckersAction::hasRemainingActions(this, state);
}

std::vector<Player> CheckersManager::getWinners() const {
    if (getLastAction().surrend) {
        Player player = getLastAction().author == players[0].id ? players[1] : players[0];
        return {player};
    }
    std::vector<Player> result;
    
    auto scores = this->getScores();
    int maxScore = INT_MIN;
    for (uint i = 0; i < scores.size(); i++)
        if (scores[i] > maxScore)
            maxScore = scores[i];

    for (uint i = 0; i < players.size(); i++)
        if (scores[i] == maxScore)
            result.push_back(players[i]);

    return result;
}