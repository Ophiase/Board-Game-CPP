#include "graphics/checkers/CheckersGame.hpp"
#include "graphics/Launcher.hpp"
#include "graphics/loot/LootGame.hpp"
#include "engine/bot/AlphaBetaStrategy.hpp"

CheckersGame::CheckersGame(Launcher *launcher, bool againstBot) :
Game{launcher, "Checkers", 1.0f}, manager{againstBot} {
    {
        auto *nPlayers = new Text{this,
            std::to_string(this->manager.players.size()) +
            " players"
        };

        nPlayers->setSizeY(0.04);
        nPlayers->center(sf::Vector2f{0.27, 0.05});
        this->addObjectToDelete(nPlayers);
    }

    {
        TextButton *surrendButton = new TextButton{this,
            "Surrend",
            [this](sf::Event) -> void {
                if (this->isFinished)
                    return;

                auto surrend = CheckersAction{
                    &this->manager, 
                    this->manager.getCurrentPlayerIndex(), 
                    this->manager.step()
                    };

                this->applyAction(surrend);
            }
        };

        surrendButton->setSizeY(0.05);
        surrendButton->center(sf::Vector2f{0.8, 0.55});
        this->addObjectToDelete(surrendButton);
    }


    this->AIinit();
    
    interactive = !manager.getCurrentPlayer().isAI;
    this->startTurn();
    
    if (!interactive)
         AIturn();
};

CheckersGame::~CheckersGame() {
    for (auto *bot : this->bots)
        delete bot;
};

void CheckersGame::startTurn() {
    this->isFinished = manager.isFinished();
    
    this->cacheAction.clear();
    this->updateBoardContent(manager.getBoard());
    this->setCurrentPlayer(manager.getCurrentPlayer().name);
    this->setScores(manager.getScores());

    if (!manager.isFinished()) {
        Cli::info(
            "\n" + Cli::separation() +
            "\n\tTurn : " + std::to_string(manager.getState().step) +
            "\n\tCurrent player : " + manager.getCurrentPlayer().name +
            "\n"
            );
		std::string playerColor = manager.getCurrentPlayer().id == 0 ? "white" : "black";
        this->setMessage("Select a " + playerColor + " pawn.");
        this->draw();
        return;
    }

    this->interactive = true;

    auto winners = this->manager.getWinners();
    if (winners.size() == 1) {            
        this->setMessage("Winner is : " + winners[0].name);
        Cli::info("\nWinner is : " + winners[0].name + "\n");
        return;
    }

    std::string winnersString = "Winners are :";
    for (auto winner : winners)
        winnersString += " " + winner.name;
    this->setMessage(winnersString);
    Cli::info("\n"+winnersString+"\n");
    this->draw();
}

// --------------------------------------------------

void CheckersGame::applyAction(CheckersAction action) {
    Cli::info(
        "Action : " + manager.getCurrentPlayer().name + " : " + 
        Cli::toString(action.jumps));

    this->manager.applyAction(action);
    interactive = !manager.getCurrentPlayer().isAI;
    this->startTurn();
    if (isFinished) return;
    
    if (!interactive)
        AIturn();
}

void CheckersGame::playAction() {
    if (!interactive) return;

    CheckersAction action{
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

void CheckersGame::cancelAction() {
    if (!interactive) return;

    if (!cacheAction.empty()) {
        Cli::debug("Cleared Action");
        cacheAction.clear();
        this->startTurn();
        return;
    }

    if (manager.step() == 0) {
        Cli::warning("Nothing to undo!");
        return;
    }

    do {
        manager.cancel();
    } while (manager.getCurrentPlayer().isAI);
    
    isFinished = false;
    this->startTurn();
}

void CheckersGame::AIinit() {
    for (auto player : this->manager.players)
        if (player.isAI) {
            //auto *strategy = 
            //new AlphaBetaStrategy<CheckersAction, Board, CheckersManager>{&this->manager, 5};

            this->bots.push_back(
                new Bot<CheckersAction, Board, CheckersManager>{
                    &this->manager, player.id, nullptr
            });
        }
 }

 void CheckersGame::AIturn() {
     this->setMessage(this->manager.getCurrentPlayer().name + "'s turn !");
     this->draw();

     Bot<CheckersAction, Board, CheckersManager> *bot;
     for (auto *x : bots)
         if (x->botId == this->manager.getCurrentPlayer().id)
            bot = x;
    
     CheckersAction action = bot->play(this->manager.getState());

     applyAction(action);
 }

// -------------------------------------------------

float diffToRotationCheck(sf::Vector2i diff) {
    if (diff == sf::Vector2i{2,2} || diff == sf::Vector2i{1,1}) return 45*1;
    if (diff == sf::Vector2i{-2,2} || diff == sf::Vector2i{-1,1}) return 45*3;
    if (diff == sf::Vector2i{-2,-2} || diff == sf::Vector2i{-1,-1}) return 45*5;
    if (diff == sf::Vector2i{2,-2} || diff == sf::Vector2i{1,-1}) return 45*7;

    throw NotImplemented();
}

void CheckersGame::updateBoardContent (Board board) {
    Game::updateBoardContent(board);
    
    //if (cacheAction.empty()) return;

    if (!this->interactive || this->isFinished) return;

    // SELECTABLE
    auto actions = CheckersAction::getActions(&this->manager, this->manager.getState());
    Combination initPositions{};
    for (auto action : actions)
        if (!initPositions.has(action.jumps[0]))
            initPositions.push_back(action.jumps[0]);


    float const selectSpace = (float)(this->checkBoardTexture.getSize().x / 10); 
    sf::RectangleShape select{sf::Vector2f{
        selectSpace, selectSpace
    }};
    
    select.setTexture(ResourcesLoader::getTexture(Texture::Selectable));
    for (auto position : initPositions) {
        float const px = (selectSpace * position.x);
        float const py = (selectSpace * position.y);
        select.setPosition(px, py);
        checkBoardTexture.draw(select);
    }
    

    // SELECTION

    float const circleSpace = (float)(this->checkBoardTexture.getSize().x / 10); 
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

    /*
        float const arrowSpace = (float)(this->checkBoardTexture.getSize().x / 10); 
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
            float const rotation = diffToRotationCheck(diff);

            arrow.setPosition(px, py);
            arrow.setRotation(rotation);
            checkBoardTexture.draw(arrow);
        }
    */
}

// --------------------------------------------------

void CheckersGame::handleCheckerBoard() {
    if (!interactive || isFinished) return;

    cacheAction.push_back(getCellPosition(manager.getBoard()));
    CheckersAction action{
        &(this->manager), manager.getCurrentPlayer().id, manager.step(),
        cacheAction
    };

    /*
    CellPieceType playerColor = manager.getCurrentPlayer().id == 0 ? CellPieceType::WhitePawn : CellPieceType::BlackPawn;
    bool const validAction = manager.canPlayAction(action);
    bool const isFirstSelection = (action.jumps.size() == 1);
    bool const isFirstSelectionCorrect = isFirstSelection &&
        manager.getBoard().getCell(action.jumps[0])
        .pieceType == playerColor;
        
    if (!(validAction || isFirstSelectionCorrect)) {
        Cli::warning("Invalid move");
        this->setMessage("Invalid move");
        cacheAction.pop_back();
        return;
    }
    */
    //Cli::debug("Cached Action : " + Cli::toString(cacheAction));   
    
    this->setMessage("Select another cell or confirm.");
    this->updateBoardContent(manager.getBoard());
}

bool CheckersGame::handleMouse(sf::Event) {
    if (!interactive) return false;

    if (mouseInsideCheckerBoard()) {
        handleCheckerBoard();
        return true;
    }

    return false;
};

bool CheckersGame::handleKeyboard(sf::Event e) {
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

bool CheckersGame::handleEvent(sf::Event e) {
    Game::handleEvent(e);

    if (e.type == sf::Event::MouseButtonPressed)
        if (handleMouse(e)) return true;
    
    if (e.type == sf::Event::KeyPressed)
        if (handleKeyboard(e)) return true;

    return false;
};

void CheckersGame::draw() {
    Game::draw();
};