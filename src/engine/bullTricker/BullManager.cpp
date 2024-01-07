#include "engine/bullTricker/BullManager.hpp"
#include "utils/NotImplemented.hpp"

BullManager::BullManager(bool againstBot) : 
    Manager<BullAction, BoardSided>{makePlayers(againstBot)} 
{
    states.push_back(BullState(
        initialBoard(), 
        ScoreList(2, 0)
        ));
};

std::vector<Player> BullManager::makePlayers(bool againstBot) 
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

BoardSided BullManager::initialBoard() {
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
        horizontalSidePieces[i][1] = SidePiece(SidePieceType::BlackSidePawn);
        horizontalSidePieces[i][6] = SidePiece(SidePieceType::WhiteSidePawn);
    }
    for (int i = 0; i < 8; i++) {
        verticalSidePieces[i][0] = SidePiece(SidePieceType::BlackSideQueen);
        verticalSidePieces[i][6] = SidePiece(SidePieceType::WhiteSideQueen);
        verticalSidePieces[i][1] = SidePiece(SidePieceType::BlackSidePawn);
        verticalSidePieces[i][5] = SidePiece(SidePieceType::WhiteSidePawn);
    }
    return BoardSided(cellPieces, horizontalSidePieces, verticalSidePieces,0);
};

bool BullManager::canPlayAction(BullState state) const {
    return BullAction::hasRemainingActions(this, state);
}