#include "graphics/loot/LootGame.hpp"
#include "graphics/Launcher.hpp"

LootGame::LootGame(Launcher *launcher) :
Game{launcher, "Loot", 1.0f}, manager{} {
    
    this->setMessage("Select a yellow pawn.");
    this->setCurrentPlayer(this->manager.getCurrentPlayerName());

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

    {
        // add a validation action button
    }
};


void handleMouse(sf::Event e) {
    (void)e;
};

void handleKeyboard(sf::Event e) {
    (void)e;
};

void LootGame::handleEvent(sf::Event e) {
    Game::handleEvent(e);

    if (e.type == sf::Event::MouseButtonPressed)
        handleMouse(e);
    
    if (e.type == sf::Event::KeyPressed)
        handleKeyboard(e);
};

void LootGame::draw() {
    Game::draw();
};