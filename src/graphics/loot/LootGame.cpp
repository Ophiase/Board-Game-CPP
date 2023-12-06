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
    this->setScores(manager.getScores());
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
    if (!interactive) return;

    if (!cacheAction.empty()) {
        Cli::debug("Cleared Action");
        cacheAction.clear();
        this->startTurn();
        return;
    }

    if (manager.step() == 0) {
        Cli::warning("Cannot undo first action!");
        setMessage("Cannot undo first action!");
        return;
    }

    manager.cancel();
    this->startTurn();
}

void LootGame::AIturn() {
    throw NotImplemented();
}

// -------------------------------------------------

float diffToRotation(sf::Vector2i diff) {
    if (diff == sf::Vector2i{2,0}) return 0;
    if (diff == sf::Vector2i{2,2}) return 45*1;
    if (diff == sf::Vector2i{0,2}) return 45*2;
    if (diff == sf::Vector2i{-2,2}) return 45*3;

    if (diff == sf::Vector2i{-2,0}) return 45*4;
    if (diff == sf::Vector2i{-2,-2}) return 45*5;
    if (diff == sf::Vector2i{0,-2}) return 45*6;
    if (diff == sf::Vector2i{2,-2}) return 45*7;

    throw NotImplemented();
}

void LootGame::updateBoardContent (Board board) {
    Game::updateBoardContent(board);
    if (cacheAction.empty()) return;

    // SELECTION

    float const circleSpace = (float)(this->checkBoardTexture.getSize().x / 8); 
    sf::RectangleShape circle{sf::Vector2f{
        circleSpace, circleSpace
    }};
    
    circle.setTexture(ResourcesLoader::getTexture(Texture::Selection));

    for (CellPosition position : cacheAction) {
        float const px = (circleSpace * position.x);
        float const py = (circleSpace * position.y);
        circle.setPosition(px, py);
        checkBoardTexture.draw(circle);
    }

    // ARROWS

    float const arrowSpace = (float)(this->checkBoardTexture.getSize().x / 8); 
    sf::RectangleShape arrow{sf::Vector2f{
        arrowSpace, arrowSpace
    }};

    arrow.setOrigin(sf::Vector2f{arrowSpace/2, arrowSpace/2});
    arrow.scale(sf::Vector2f{1.5, 1.5});
    
    arrow.setTexture(ResourcesLoader::getTexture(Texture::Arrow));
    for (uint i = 1; i < cacheAction.size(); i++) {
        CellPosition position = (cacheAction[i] + cacheAction[i-1]) / 2;
        CellPosition diff = (cacheAction[i] - cacheAction[i-1]);

        float const px = (arrowSpace * (position.x + 0.5));
        float const py = (arrowSpace * (position.y + 0.5));
        float const rotation = diffToRotation(diff);

        arrow.setPosition(px, py);
        arrow.setRotation(rotation);
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
    if (e.key.code == sf::Keyboard::Backspace)
        cancelAction();
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