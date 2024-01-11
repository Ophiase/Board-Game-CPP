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

bool BullManager::repetePatCheck() {
    const uint N_REPETITION_FOR_PAT = 7;
    const uint N_TURNS = N_REPETITION_FOR_PAT*4;
    Cli::debug("Verify pat");
    
    if (actions.size() < N_REPETITION_FOR_PAT*4)
        return false;

    Cli::debug("- Enough actions");

    auto oldScore = this->states[this->step() - N_TURNS - 1].scores;
    if (this->getScores() != oldScore)
        return false;

    Cli::debug("- Score remains");
    
    BullAction current = this->getLastAction();
    for (uint i = 0; i < N_TURNS; i += 4){
        Cli::debug(" - check ? : " + std::to_string(i) + " / " + std::to_string(N_TURNS));
        auto direct = actions[this->step() - i - 1];
        auto indirect = actions[this->step() - i - 1 - 2];

        if ((direct != current) || !(direct.revEqual(indirect)))
            return false;
    }

    Cli::debug("checked");

    return true;
}

bool BullManager::canPlayAction(BullState state) const {
    if (state == this->getState() && this->step() > 0) {
        if (this->getLastAction().surrend)
            return false;

        if (this->getLastAction().askForPat)
            return false;
    }

    return BullAction::hasRemainingActions(this, state);
}

std::vector<Player> BullManager::getWinners() const {
    if (getLastAction().surrend) {
        Player player = getLastAction().author == players[0].id ? players[1] : players[0];
        return {player};
    }
    BoardSided board = this->getBoard();
    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {
            if (board.getCell(x,y).pieceType == CellPieceType::BlackKing)
                if (getLastAction().isSurrounded(this, this->getState(), CellPosition{x,y}))
                    return {players[0]};
            if (board.getCell(x,y).pieceType == CellPieceType::WhiteKing)
                if (getLastAction().isSurrounded(this, this->getState(), CellPosition{x,y}))
                    return {players[1]};
        }
    }
    return {players[0], players[1]};
}