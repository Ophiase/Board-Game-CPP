#include "graphics/Menu.hpp"
#include "graphics/screen/Text.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"
#include "graphics/Launcher.hpp"
#include "utils/Cli.hpp"

sf::Vector2f areaPosition{0.1, 0.15};
sf::Vector2f areaSize{0.8, 0.6};

std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(Launcher *launcher) : Screen{launcher} {
    Text *text = new Text{
        this, "Hello world!", sf::Vector2f{0, 0}, 0.1};
    text->center(sf::Vector2f{0.5, 0.5});

    TextButton *button = new TextButton{
        this, "Button",
        [](sf::Event e) -> void {
            (void)e;
            Cli::debug("Click !");
        }, [](sf::Event e) -> void {
            (void)e;
            Cli::debug("Hover !");
        }};
    button->center(sf::Vector2f{0.5, 0.3});
};

void Menu::draw() {
    Screen::draw();
};