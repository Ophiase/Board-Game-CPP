#include "graphics/loot/LootGame.hpp"
#include "graphics/Launcher.hpp"

LootGame::LootGame(Launcher *launcher) :
Game{launcher, "Loot"}, manager{} {
    
    {
        this->setCurrentPlayer(this->manager.getCurrentPlayerName());
    }

    {
        auto nPlayers = new Text{this,
            std::to_string(this->manager.players.size()) +
            " players"
        };  // TODO GARBAGE

        nPlayers->setSizeY(0.04);
        nPlayers->center(sf::Vector2f{0.27, 0.05});
        this->setScores(std::vector<int>{0, 0}); // TODO
    }

    {
        this->updateBoardContent(manager.getConfiguration());
    }
};

void LootGame::draw() {
    Game::draw();
};