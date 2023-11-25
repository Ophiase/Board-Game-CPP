#include "graphics/Menu.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"


std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(Launcher *launcher) : Screen{launcher} {
    TextButton *textButton = new TextButton{
        this, "Hello world!", 
        sf::FloatRect{0.0, 0.0, 1.0, 1.0},
        ResourcesLoader::getFont(Font::OpenSansBold)
    };
    
    //this->addObjectToDelete(textButton);  
};