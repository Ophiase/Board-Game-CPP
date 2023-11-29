#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

std::vector<
    std::tuple<Player, std::string>
    > LootManager::makePlayers(int n) {
    
    std::vector<std::tuple<Player, std::string>> output;

    for (int i = 0; i < n; i++)
        output.push_back(std::make_tuple(
            i,
            "Player_" + std::to_string(i)
        ));

    return output;
}

LootManager::LootManager(int nPlayers) : 
    Manager<LootAction>{makePlayers(nPlayers)} {
    configurations.push_back(initialBoard());
};

Board LootManager::initialBoard() {
        std::vector<std::vector<CellPiece>> cellPieces{
            8, std::vector<CellPiece>(8)
        };
        return Board(cellPieces, 0);
};


std::vector<LootAction> LootManager::getActions() {
    throw NotImplemented();
};

bool LootManager::canPlay(LootAction action) {
    (void)action;
    throw NotImplemented();
};

void LootManager::playAction(LootAction action) {
    (void)action;
    throw NotImplemented();
};

bool LootManager::actionEquivalence(
    LootAction actionA, LootAction actionB) {
    
    (void)actionA;
    (void)actionB;
    throw NotImplemented();
}