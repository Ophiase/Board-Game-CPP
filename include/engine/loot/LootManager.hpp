#pragma once

#include "engine/Board.hpp"
#include "engine/Manager.hpp"
#include "LootAction.hpp"

class LootManager : public Manager<LootAction> {
    private:
        Board initialBoard() override;
        
    public:
        LootManager(int nPlayers = 2);

        std::vector<LootAction> getActions() override;
        bool canPlay(LootAction action) override;
        void playAction(LootAction action) override;
        
        static std::vector<
            std::tuple<Player, std::string>
            > makePlayers(int n);
};