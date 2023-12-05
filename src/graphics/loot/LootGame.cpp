#include "graphics/loot/LootGame.hpp"
#include "graphics/Launcher.hpp"

LootGame::LootGame(Launcher *launcher) :
Game{launcher, "Loot", 1.0f}, manager{} {
    {
        auto nPlayers = new Text{this,
            std::to_string(this->manager.players.size()) +
            " players"
        };

        nPlayers->setSizeY(0.04);
        nPlayers->center(sf::Vector2f{0.27, 0.05});
        this->setScores(std::vector<int>{0, 0});
        this->addObjectToDelete(nPlayers);
    }



    this->startTurn();

    interactive = ! manager.getCurrentPlayer().isAI;
    if (!interactive)
        AIturn();
};

void LootGame::startTurn() {
    this->cacheAction.clear();
    this->updateBoardContent(manager.getConfiguration());
    this->setCurrentPlayer(manager.getCurrentPlayer().name);
    this->setScores(std::vector<int>{0, 0}); // TODO
    this->setMessage("Select a yellow pawn.");
}

// --------------------------------------------------

void LootGame::playAction() {
    if (!interactive) return;

    Cli::debug("I'm playing : " + Cli::toString(cacheAction));
    LootAction action{
        manager.getCurrentPlayer().id,
        cacheAction
    };

    this->manager.applyAction(action);
    this->startTurn();


    interactive = ! manager.getCurrentPlayer().isAI;
    if (!interactive)
        AIturn();
}

void LootGame::cancelAction() {
    throw NotImplemented();
}

void LootGame::AIturn() {
    throw NotImplemented();
}

// --------------------------------------------------

void LootGame::handleCheckerBoard() {
    if (!interactive) return;

    cacheAction.push_back(getCellPosition());
    
    LootAction action{
        manager.getCurrentPlayer().id, 
        cacheAction
    };
    
    if (!manager.canPlayAction(action)) {
        Cli::warning("Invalid move");
        this->setMessage("Invalid move");
        cacheAction.pop_back();
        return;
    }
    
    Cli::debug("Current Action : " + Cli::toString(cacheAction));
    
    this->setMessage("Select another cell or play.");
}

void LootGame::handleMouse(sf::Event e) {
    (void)e;
    
    if (!interactive) return;

    if (mouseInsideCheckerBoard())
        handleCheckerBoard();
    
};

void LootGame::handleKeyboard(sf::Event e) {
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