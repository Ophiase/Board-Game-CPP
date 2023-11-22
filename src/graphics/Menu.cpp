#include "graphics/Menu.hpp"
#include "graphics/screen/TextButton.hpp"
#include "graphics/ResourcesLoader.hpp"

std::string Menu::getTitle() { return "Menu"; }

Menu::Menu(sf::RenderWindow & window) : Screen{window} {
    auto text = sf::Text{
        "Hello world!", 
        ResourcesLoader::getFont(Font::OpenSansExtraBold), 100U
    };

    float textScale = 0.0006;
    text.setScale(sf::Vector2f(textScale, textScale));
        
    TextButton textButton{(window), text};
    
    this->addDrawing(textButton);
    this->addHandler(textButton);
};
