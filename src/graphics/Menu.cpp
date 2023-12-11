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

void Menu::lootOptionInit() {
    TextButton *playButton = new TextButton{
    &lootOption, "Start Loot",
    [this](sf::Event) -> void {
        this->successor = new LootGame(this->getLauncher());
    }};

    playButton->center(sf::Vector2f{0.5, 0.60});
    lootOption.addObjectToDelete(playButton);
}

void Menu::checkersOptionInit() {
    TextButton *playButton = new TextButton{
    &checkersOption, "Start Checkers",
    [this](sf::Event) -> void {
        this->successor = new CheckersGame(this->getLauncher());
    }};

    playButton->center(sf::Vector2f{0.5, 0.60});
    checkersOption.addObjectToDelete(playButton);
}

void Menu::bullTrickerOptionInit() {
    TextButton *playButton = new TextButton{
    &bullTrickerOption, "Start BullTricker",
    [this](sf::Event) -> void {
        this->successor = new BullTrickerGame(this->getLauncher());
    }};

    playButton->center(sf::Vector2f{0.5, 0.60});
    bullTrickerOption.addObjectToDelete(playButton);
}

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
        exitButton->center(0.5, 0.3);
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