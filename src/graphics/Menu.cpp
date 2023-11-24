#include "graphics/Menu.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "geometry/Geometry.hpp"

std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(Launcher *launcher) : Screen{launcher} {
    auto text = sf::Text{
        "Hello world!", 
        ResourcesLoader::getFont(Font::OpenSansExtraBold), 100U
    };

    float textScale = 0.0006;
    text.setScale(sf::Vector2f(textScale, textScale));
    
    // todo deconstruct
    TextButton *textButton = new TextButton{this, text};
    
    this->addDrawing(textButton);
    this->addHandler(textButton);
};
