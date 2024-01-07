#include "graphics/bullTricker/BullTrickerGame.hpp"
#include "graphics/Launcher.hpp"
#include "engine/bot/AlphaBetaStrategy.hpp"

BullTrickerGame::BullTrickerGame(Launcher *launcher, bool againstBot) :
Game{launcher, "BullTricker", 0.7f}, manager{againstBot} {
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
                /* TODO, ADD surrend to actions
                auto surrend = BullAction{
                    &this->manager, 
                    this->manager.getCurrentPlayerIndex(), 
                    this->manager.step()
                    };
                
                this->applyAction(surrend);
                */
            }
        };

        surrendButton->setSizeY(0.05);
        surrendButton->center(sf::Vector2f{0.8, 0.55});
        this->addObjectToDelete(surrendButton);
    }

    this->AIinit();
    
    interactive = !manager.getCurrentPlayer().isAI;
    this->startTurn();
    return;
    
    if (!interactive)
         AIturn();
};

BullTrickerGame::~BullTrickerGame() {
    for (auto *bot : this->bots)
        delete bot;
};

void BullTrickerGame::startTurn() {
    this->isFinished = manager.isFinished();
    
    this->cachedCellAction.clear();
    this->cachedSideAction.clear();
    this->updateBoardSidedContent(manager.getBoard());
    this->setCurrentPlayer(manager.getCurrentPlayer().name);
    this->setScores(manager.getScores());

    if (!manager.isFinished()) {
        Cli::info(
            "\t" + Cli::separation() + "\n" +
            "\t\x1B[35mTurn \x1B[0m: " + std::to_string(manager.getState().step) +
            "\n\t\x1B[34mCurrent player \x1B[0m: " + manager.getCurrentPlayer().name +
            "\n\t" + Cli::separation() + "\n"
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
        Cli::info("\n\t\x1B[35mWinner is\x1B[0m : " + winners[0].name + "\n");
        return;
    }

    std::string winnersString = "";
    for (auto winner : winners)
        winnersString += " " + winner.name;
    this->setMessage("Winners are :"+winnersString);
    Cli::info("\n\t\x1B[35mWinner are\x1B[0m : "+winnersString+"\n");
    this->draw();
}

// --------------------------------------------------

void BullTrickerGame::applyAction(BullAction action) {
    if (action.isSidePath)
        Cli::info(
            "Action : " + manager.getCurrentPlayer().name + " : " + 
            Cli::toString(action.sideJumps) +"\n");
    else
        Cli::info(
            "Action : " + manager.getCurrentPlayer().name + " : " + 
            Cli::toString(action.cellJumps) +"\n");

    this->manager.applyAction(action);
    interactive = !manager.getCurrentPlayer().isAI;
    this->startTurn();
    if (isFinished) return;
    
    if (!interactive)
        AIturn();
}

void BullTrickerGame::playAction() {
    if (!interactive) return;

    BullAction action = cachedCellAction.empty() ? 
        BullAction{
            &(this->manager),
            manager.getCurrentPlayer().id,
            manager.step(),
            cachedSideAction
        } : BullAction{
            &(this->manager),
            manager.getCurrentPlayer().id,
            manager.step(),
            cachedCellAction
    };

    if (!manager.canPlayAction(action)) {
        this->setMessage("Invalid action.");
        Cli::debug("Invalid action.");
        return;
    }

    applyAction(action);
}

void BullTrickerGame::cancelAction() {
    if (!interactive) return;

    if (!cachedCellAction.empty() || !cachedSideAction.empty()) {
        Cli::debug("Cleared Action");
        cachedCellAction.clear();
        cachedSideAction.clear();
        this->updateBoardSidedContent(manager.getBoard());
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

void BullTrickerGame::AIinit() {
    for (auto player : this->manager.players)
        if (player.isAI) {
            auto *strategy = 
            new AlphaBetaStrategy<BullAction, BoardSided, BullManager>{&this->manager, 3};

            this->bots.push_back(
                new Bot<BullAction, BoardSided, BullManager>{
                    &this->manager, player.id, strategy
            });
        }
 }

 void BullTrickerGame::AIturn() {
     this->setMessage(this->manager.getCurrentPlayer().name + "'s turn !");
     this->draw();

     Bot<BullAction, BoardSided, BullManager> *bot;
     for (auto *x : bots)
         if (x->botId == this->manager.getCurrentPlayer().id)
            bot = x;
    
     BullAction action = bot->play(this->manager.getState());

     applyAction(action);
 }

// -------------------------------------------------

void BullTrickerGame::updateBoardSidedContent (BoardSided board) {
    Game::updateBoardSidedContent(board);

    //if (cacheAction.empty()) return;
    if (!this->interactive || this->isFinished) return;
}

// --------------------------------------------------

void BullTrickerGame::handleCheckerBoard() {
    if (!interactive || isFinished) return;

    if (mouseInsideCheckerBoard()) {
        if (mouseOnCase(manager.getBoard(), true)) {
            if (!cachedSideAction.empty())
                cachedSideAction.clear();

            auto pos = getCellPosition(manager.getBoard());

            cachedCellAction.push_back(pos);            
            Cli::debug("Clicked on case : " + Cli::toString(pos));
        } else {
            if (!cachedCellAction.empty())
                cachedCellAction.clear();

            auto pos = getSidePosition(manager.getBoard());
            
            cachedSideAction.push_back(pos);
            Cli::debug("Clicked on side :     " + Cli::toString(pos));
        }
    }


    this->setMessage("Select another cell or confirm.");
    this->updateBoardSidedContent(manager.getBoard());
}

bool BullTrickerGame::handleMouse(sf::Event) {
    if (!interactive) return false;

    if (mouseInsideCheckerBoard()) {
        handleCheckerBoard();
        return true;
    }

    return false;
};

bool BullTrickerGame::handleKeyboard(sf::Event e) {
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

bool BullTrickerGame::handleEvent(sf::Event e) {
    Game::handleEvent(e);

    if (e.type == sf::Event::MouseButtonPressed)
        if (handleMouse(e)) return true;
    
    if (e.type == sf::Event::KeyPressed)
        if (handleKeyboard(e)) return true;

    return false;
};

void BullTrickerGame::draw() {
    Game::draw();
};