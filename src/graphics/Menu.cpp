#include "graphics/Menu.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"


std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(Launcher *launcher) : Screen{launcher} {
    //std::cout << std::endl;

    TextButton *textButton = new TextButton{
        this, "Hello!", 
        sf::FloatRect{0.3, 0.2, 0.4, 0.6},
        ResourcesLoader::getFont(Font::OpenSansBold)
    };
    
    //this->addObjectToDelete(textButton);  
};