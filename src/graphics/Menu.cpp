#include "graphics/Menu.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"
#include "graphics/Launcher.hpp"

sf::Vector2f areaPosition{0.1, 0.15};
sf::Vector2f areaSize{0.8, 0.6};

std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(Launcher *launcher) : Screen{launcher} {
    std::vector<std::string> buttons = {
        "a string", "another", "jdia", "YES!"
        };

    for (int i = 0; i < buttons.size(); i++) {
        float sy = areaSize.y / (float)buttons.size();
        float py = areaPosition.y + sy * (float)i;

        TextButton *textButton = new TextButton{
            this, buttons[i], 
            sf::FloatRect{
                areaPosition.x, py, 
                areaSize.x, sy
                },
            ResourcesLoader::getFont(Font::OpenSansBold)
        };
        
        //this->addObjectToDelete(textButton);  
    }

};

void Menu::draw() {
    /*
    sf::RectangleShape rect;
    rect.setPosition(areaPosition);
    rect.setSize(areaSize);
    rect.setFillColor(sf::Color::Blue);

    getRenderWindow().draw(rect);
    */

    std::vector<sf::Color> colors = {
        sf::Color::Blue, sf::Color::Red, 
        sf::Color::Green, sf::Color::Black
    };

    for (int i = 0; i < colors.size(); i++) {
        float sy = areaSize.y / (float)colors.size();
        float py = areaPosition.y + sy * (float)i;

        sf::RectangleShape rect;
        rect.setPosition(areaPosition.x, py);
        rect.setSize(sf::Vector2f{areaSize.x, sy});
        rect.setFillColor(colors[i]);

        getRenderWindow().draw(rect);    
    }

    Screen::draw();
};