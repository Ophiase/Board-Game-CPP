#include "engine/loot/LootManager.hpp"
#include "utils/NotImplemented.hpp"

LootManager::LootManager(int nPlayers) : Manager<LootAction>{nPlayers} {
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