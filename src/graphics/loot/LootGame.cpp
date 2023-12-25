#include "graphics/loot/LootGame.hpp"
#include "graphics/Launcher.hpp"

LootGame::LootGame(Launcher *launcher, int nPlayers, int nBots) :
Game{launcher, "Loot", 1.0f}, manager{nPlayers, nBots} {
    {
        auto nPlayers = new Text{this,
            std::to_string(this->manager.players.size()) +
            " players"
        };

        nPlayers->setSizeY(0.04);
        nPlayers->center(sf::Vector2f{0.27, 0.05});
        this->addObjectToDelete(nPlayers);
    }

    this->AIinit();
    this->startTurn();

    interactive = ! manager.getCurrentPlayer().isAI;
    if (!interactive)
        AIturn();
};

LootGame::~LootGame() {
    for (auto *bot : this->bots)
        delete bot;
};

void LootGame::startTurn() {
    this->cacheAction.clear();
    this->updateBoardContent(manager.getConfiguration());
    this->setCurrentPlayer(manager.getCurrentPlayer().name);
    this->setScores(manager.getScores());

    if (!this->manager.isFinished()) {
        Cli::info("Current player : " + manager.getCurrentPlayer().name);
        this->setMessage("Select a yellow pawn.");
        this->draw();
        return;
    }

    auto winners = this->manager.getWinners();
    if (winners.size() == 1) {            
        this->setMessage("Winner is : " + winners[0].name);
        Cli::info("Winner is : " + winners[0].name);
        return;
    }

    std::string winnersString = "Winner are :";
    for (auto winner : winners)
        winnersString += " " + winner.name;
    this->setMessage(winnersString);
    Cli::info(winnersString);
    this->draw();
}

// --------------------------------------------------

void LootGame::applyAction(LootAction action) {
    Cli::info(
        "Action : " + manager.getCurrentPlayer().name + " : " + 
        Cli::toString(action.jumps));

    this->manager.applyAction(action);
    this->startTurn();

    if (this->manager.isFinished()) {
        interactive = false;
        return;
    }

    interactive = !manager.getCurrentPlayer().isAI;
    if (!interactive)
        AIturn();
}

void LootGame::playAction() {
    if (!interactive) return;

    LootAction action{
        &(this->manager),
        manager.getCurrentPlayer().id,
        manager.step(),
        cacheAction
    };

    if (!manager.canPlayAction(action)) {
        this->setMessage("Invalid action.");
        Cli::debug("Invalid action.");
        return;
    }

    applyAction(action);
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

void LootGame::AIinit() {
    for (auto player : this->manager.players)
        if (player.isAI)
            this->bots.push_back(
                new Bot<LootAction, Board, LootManager>{
                    &this->manager, player.id
            });
}

void LootGame::AIturn() {
    this->setMessage(this->manager.getCurrentPlayer().name + 
        "'s turn !");
    this->draw();

    Bot<LootAction, Board, LootManager> *bot;
    for (auto *x : bots)
        if (x->botId == this->manager.getCurrentPlayer().id)
           bot = x;

    LootAction action = bot->play(
        this->manager.step(),
        this->manager.getConfiguration()
    );

    applyAction(action);
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
    arrow.scale(sf::Vector2f{1.8, 1.8});
    
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
        &(this->manager), manager.getCurrentPlayer().id, manager.step(),
        cacheAction
    };

    bool const validAction = manager.canPlayAction(action);
    bool const isFirstSelection = (action.jumps.size() == 1);
    bool const isFirstSelectionCorrect = isFirstSelection &&
        manager.getConfiguration().getCell(action.jumps[0])
        .pieceType == CellPieceType::YellowPawn;
        
    if (!(validAction || isFirstSelectionCorrect)) {
        Cli::warning("Invalid move");
        this->setMessage("Invalid move");
        cacheAction.pop_back();
        return;
    }
    
    //Cli::debug("Cached Action : " + Cli::toString(cacheAction));   
    
    this->setMessage("Select another cell or play.");
    this->updateBoardContent(manager.getConfiguration());
}

bool LootGame::handleMouse(sf::Event) {
    if (!interactive) return false;

    if (mouseInsideCheckerBoard()) {
        handleCheckerBoard();
        return true;
    }

    return false;
};

bool LootGame::handleKeyboard(sf::Event e) {
    if (e.key.code == sf::Keyboard::Return) {
        playAction();
        return true;
    }
    
    if (e.key.code == sf::Keyboard::Backspace) {
        cancelAction();
        return true;
    }

    return false;
};

bool LootGame::handleEvent(sf::Event e) {
    Game::handleEvent(e);

    if (e.type == sf::Event::MouseButtonPressed)
        if (handleMouse(e)) return true;
    
    if (e.type == sf::Event::KeyPressed)
        if (handleKeyboard(e)) return true;

    return false;
};

void LootGame::draw() {
    Game::draw();
};