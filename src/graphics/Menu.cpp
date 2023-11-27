#include "graphics/Menu.hpp"
#include "graphics/screen/Text.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"
#include "graphics/Launcher.hpp"

sf::Vector2f areaPosition{0.1, 0.15};
sf::Vector2f areaSize{0.8, 0.6};

std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(Launcher *launcher) : Screen{launcher} {
    /*
    std::vector<std::string> buttons = {
        "", "Option 1", "Option 2", "Option 3", ""
        };

    for (uint i = 0; i < buttons.size(); i++) {
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
    */

   //Text text{this, "Hello world!", sf::Vector2f{0, 0}, 0.2};
};

void Menu::draw() {
    Screen::draw();
};