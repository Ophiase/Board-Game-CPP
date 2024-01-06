#include "graphics/Menu.hpp"
#include "graphics/screen/Text.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"
#include "graphics/Launcher.hpp"
#include "utils/Cli.hpp"
#include "graphics/bullTricker/BullTrickerGame.hpp"
#include "graphics/checkers/CheckersGame.hpp"
#include "graphics/loot/LootGame.hpp"

#include <tuple>

sf::Vector2f areaPosition{0.1, 0.15};
sf::Vector2f areaSize{0.8, 0.6};

void Menu::gameSelectionInit() {
    TextButton *lootGameButton = new TextButton{
        &gameSelection, "Loot",
        [this](sf::Event) -> void {
            this->setContext(MenuContext::LootOption);
        }};

    TextButton *checkersGameButton = new TextButton{
        &gameSelection, "Checkers",
        [this](sf::Event) -> void {
            this->setContext(MenuContext::CheckersOption);
        }};

    TextButton *bullTrickerGameButton = new TextButton{
        &gameSelection, "BullTricker",
        [this](sf::Event) -> void {
            this->setContext(MenuContext::BullTrickerOption);
        }};

    lootGameButton->center(sf::Vector2f{0.5, 0.30});
    checkersGameButton->center(sf::Vector2f{0.5, 0.45});
    bullTrickerGameButton->center(sf::Vector2f{0.5, 0.60});

    gameSelection.addObjectToDelete(lootGameButton);
    gameSelection.addObjectToDelete(checkersGameButton);
    gameSelection.addObjectToDelete(bullTrickerGameButton);
}

// ---------------------------------
// Loot Options

void Menu::lootEditNPlayers(int n) {
    int newValue = std::max(1, n);
    
    this->lootNPlayers = newValue;
    this->lootNplayersText.setText(
        std::to_string(newValue));

    if (newValue < 2)
        lootEditNBots(std::max(1, this->lootNBots));
}

void Menu::lootEditNBots(int n) {
    int newValue = std::max(0, n);

    this->lootNBots = newValue;
    this->lootnBotsText.setText(std::to_string(newValue));

    if (newValue + this->lootNPlayers < 2)
        lootEditNPlayers(2);
}

void Menu::lootOptionInit() {
    TextButton *playButton = new TextButton{
    &lootOption, "Start Loot",
    [this](sf::Event) -> void {
        this->successor = new LootGame(
            this->getLauncher(), 
            this->lootNPlayers, this->lootNBots
            );
    }};
    playButton->center(sf::Vector2f{0.5, 0.60});
    lootOption.addObjectToDelete(playButton);

    {
        Text *nPlayers = new Text{&lootOption, "players : "};
        nPlayers->setSizeY(0.05);
        nPlayers->center(0.40, 0.35);
        lootOption.addObjectToDelete(nPlayers);

        this->lootNplayersText.setSizeY(0.05);
        this->lootNplayersText.setPosition(
            nPlayers->getPosition().x + nPlayers->getSize().x,
            nPlayers->getPosition().y
            );

        TextButton *minusPlayer = new TextButton{&lootOption, "-",
            [this](sf::Event) -> void {
                this->lootEditNPlayers(this->lootNPlayers - 1);
            }
        };
        minusPlayer->setSizeY(0.05);
        minusPlayer->setPosition(
            lootNplayersText.getPosition().x + lootNplayersText.getSize().x,
            lootNplayersText.getPosition().y
        );
        lootOption.addObjectToDelete(minusPlayer);

        TextButton *plusPlayer = new TextButton{&lootOption, "+",
            [this](sf::Event) -> void {
                this->lootEditNPlayers(this->lootNPlayers + 1);
            }
        };
        plusPlayer->setSizeY(0.05);
        plusPlayer->setPosition(
            minusPlayer->getPosition().x + minusPlayer->getSize().x,
            minusPlayer->getPosition().y
        );
        lootOption.addObjectToDelete(plusPlayer);
    }

    {
        Text *nBots = new Text{&lootOption, "bots : "};
        nBots->setSizeY(0.05);
        nBots->center(0.40, 0.40);
        lootOption.addObjectToDelete(nBots);

        this->lootnBotsText.setSizeY(0.05);
        this->lootnBotsText.setPosition(
            nBots->getPosition().x + nBots->getSize().x,
            nBots->getPosition().y
            );

        TextButton *minusBot = new TextButton{&lootOption, "-",
            [this](sf::Event) -> void {
                this->lootEditNBots(this->lootNBots - 1);
            }
        };
        minusBot->setSizeY(0.05);
        minusBot->setPosition(
            lootnBotsText.getPosition().x + lootnBotsText.getSize().x,
            lootnBotsText.getPosition().y
        );
        lootOption.addObjectToDelete(minusBot);

        TextButton *plusBot = new TextButton{&lootOption, "+",
            [this](sf::Event) -> void {
                this->lootEditNBots(this->lootNBots + 1);
            }
        };
        plusBot->setSizeY(0.05);
        plusBot->setPosition(
            minusBot->getPosition().x + minusBot->getSize().x,
            minusBot->getPosition().y
        );
        lootOption.addObjectToDelete(plusBot);
    }
}

// ---------------------------------
// Checker Options

void Menu::checkersOptionInit() {
    TextButton *playButtonBot = new TextButton{
    &checkersOption, "Start Checkers (vs bot)",
    [this](sf::Event) -> void {
        this->successor = new CheckersGame(
            this->getLauncher(), true);
    }};

    playButtonBot->setSizeY(0.05);
    playButtonBot->center(sf::Vector2f{0.5, 0.50});
    checkersOption.addObjectToDelete(playButtonBot);

    TextButton *playButton = new TextButton{
    &checkersOption, "Start Checkers (vs player)",
    [this](sf::Event) -> void {
        this->successor = new CheckersGame(
            this->getLauncher(), false);
    }};

    playButton->setSizeY(0.05);
    playButton->center(sf::Vector2f{0.5, 0.60});
    checkersOption.addObjectToDelete(playButton);
}

// ---------------------------------
// BullTricker Options

void Menu::bullTrickerOptionInit() {
    TextButton *playButton = new TextButton{
    &bullTrickerOption, "Start BullTricker",
    [this](sf::Event) -> void {
        this->successor = new BullTrickerGame(this->getLauncher());
    }};

    playButton->center(sf::Vector2f{0.5, 0.60});
    bullTrickerOption.addObjectToDelete(playButton);
}

// ---------------------------------


Menu::Menu(Launcher *launcher) : 
    Screen{launcher, "Menu"},
    gameSelection{this},
    lootOption{this},
    checkersOption{this},
    bullTrickerOption{this}
{
    this->setContext(MenuContext::GameSelection);

    gameSelectionInit();

    for (auto context : this->getAllContext()) {
        if (context == &gameSelection)
            continue;

        TextButton *exitButton = new TextButton {
            context, "Back to Menu",
            [this](sf::Event) -> void {
                this->setContext(MenuContext::GameSelection);
            }
        };

        exitButton->setSizeY(0.05);
        exitButton->center(0.5, 0.25);
        context->addObjectToDelete(exitButton);
    }

    lootOptionInit();
    checkersOptionInit();
    bullTrickerOptionInit();
};

void Menu::draw() {
    Screen::draw();
};


Container *Menu::getContext(MenuContext context) {
    switch (context) {
        case MenuContext::GameSelection :
            return &this->gameSelection;
        case MenuContext::LootOption :
            return &this->lootOption;
        case MenuContext::CheckersOption :
            return &this->checkersOption;
        case MenuContext::BullTrickerOption :
            return &this->bullTrickerOption;
        default:
            throw NotImplemented();
    }
}

std::vector<Container*> Menu::getAllContext() {
    return std::vector<Container*> {
        &this->gameSelection,
        &this->lootOption,
        &this->checkersOption,
        &this->bullTrickerOption
    };
}

void Menu::setContext(Container *currentContext) {
    auto allContext = this->getAllContext();
    for (auto candidateContext : allContext)
        candidateContext->setEnabled(candidateContext == currentContext);
    
    this->draw();
}

void Menu::setContext(MenuContext context) {
    auto allContext = this->getAllContext();
    auto currentContext = this->getContext(context);

    for (auto candidateContext : allContext)
        candidateContext->setEnabled(candidateContext == currentContext);
}