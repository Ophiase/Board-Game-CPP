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

Menu::Menu(Launcher *launcher) : Screen{launcher, "Menu"} {
    TextButton *lootGameButton = new TextButton{
        this, "Loot",
        [this](sf::Event e) -> void {
            (void)e;
            this->successor = new LootGame(this->getLauncher());
        }};

    TextButton *checkersGameButton = new TextButton{
        this, "Checkers",
        [this](sf::Event e) -> void {
            (void)e;
            this->successor = new CheckersGame(this->getLauncher());
        }};

    TextButton *bullTrickerGameButton = new TextButton{
        this, "BullTricker",
        [this](sf::Event e) -> void {
            (void)e;
            this->successor = new BullTrickerGame(this->getLauncher());
        }};

    lootGameButton->center(sf::Vector2f{0.5, 0.30});
    checkersGameButton->center(sf::Vector2f{0.5, 0.45});
    bullTrickerGameButton->center(sf::Vector2f{0.5, 0.60});
};

void Menu::draw() {
    Screen::draw();
};