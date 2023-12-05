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

    Cli::debug(manager.getCurrentPlayer().name + " : " + Cli::toString(cacheAction));
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

// -------------------------------------------------

void LootGame::updateBoardContent (Board board) {
    Game::updateBoardContent(board);
    if (cacheAction.empty()) return;

    // SELECTION

    float circleSpace = (float)(this->checkBoardTexture.getSize().x / 8); 
    float circleScale = 1.0;
    float circleSize = circleSpace*circleScale;

    sf::RectangleShape circle{sf::Vector2f{
        circleSize, circleSize
    }};
    
    circle.setTexture(ResourcesLoader::getTexture(Texture::Selection));

    float offset = circleSize / 2.0;
    for (CellPosition position : cacheAction) {
        float px = (circleSpace * (position.x + 0.5)) - offset;
        float py = (circleSpace * (position.y + 0.5)) - offset;

        circle.setPosition(px, py);
        checkBoardTexture.draw(circle);
    }

    // ARROWS

    float arrowSpace = (float)(this->checkBoardTexture.getSize().x / 8); 
    float arrowScale = 1.0;
    float arrowSize = arrowSpace*arrowScale;

    sf::RectangleShape arrow{sf::Vector2f{
        arrowSize, arrowSize
    }};
    
    arrow.setTexture(ResourcesLoader::getTexture(Texture::Arrow));
    for (uint i = 1; i < cacheAction.size(); i++) {
        CellPosition position = (cacheAction[i] + cacheAction[i-1]) / 2;

        float px = (arrowSpace * (position.x + 0.5)) - offset;
        float py = (arrowSpace * (position.y + 0.5)) - offset;

        arrow.setPosition(px, py);
        checkBoardTexture.draw(arrow);
    }
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
    
    Cli::debug("Cached Action : " + Cli::toString(cacheAction));   
    this->setMessage("Select another cell or play.");
    this->updateBoardContent(manager.getConfiguration());
}

void LootGame::handleMouse(sf::Event e) {
    (void)e;
    
    if (!interactive) return;

    if (mouseInsideCheckerBoard())
        handleCheckerBoard();
    
};

void LootGame::handleKeyboard(sf::Event e) {
    if (e.key.code == sf::Keyboard::Return)
        playAction();
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