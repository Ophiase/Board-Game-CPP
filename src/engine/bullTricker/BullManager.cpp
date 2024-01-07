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
        8, std::vector<SidePiece>(7)
    };
    std::vector<std::vector<SidePiece>> verticalSidePieces{
        7, std::vector<SidePiece>(8)
    };

    cellPieces[0][3] = CellPiece(CellPieceType::BlackKing);
    cellPieces[6][3] = CellPiece(CellPieceType::WhiteKing);

    for (int x = 0; x < 7; x++) {
        horizontalSidePieces[1][x] = SidePiece(SidePieceType::BlackSidePawn);
        horizontalSidePieces[6][x] = SidePiece(SidePieceType::WhiteSidePawn);
    }
    
    for (int x = 0; x < 8; x++) {
        verticalSidePieces[0][x] = SidePiece(SidePieceType::BlackSideQueen);
        verticalSidePieces[6][x] = SidePiece(SidePieceType::WhiteSideQueen);
        verticalSidePieces[1][x] = SidePiece(SidePieceType::BlackSidePawn);
        verticalSidePieces[5][x] = SidePiece(SidePieceType::WhiteSidePawn);
    }
    return BoardSided(cellPieces, horizontalSidePieces, verticalSidePieces,0);
};

bool BullManager::canPlayAction(BullState state) const {
    return BullAction::hasRemainingActions(this, state);
}