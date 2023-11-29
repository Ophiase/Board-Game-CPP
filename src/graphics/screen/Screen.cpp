#include "graphics/screen/Screen.hpp"
#include "graphics/screen/Text.hpp"

Screen::Screen(Launcher *launcher, std::string title) : 
Container{launcher}, title{title} {
    Text *textTitle = new Text(this, title);
    //textTitle->center(sf::Vector2f{0.5, 0.05});
    textTitle->fit(sf::FloatRect{
        0.35, 0.0,
        0.3, 0.1
    });
    // TODO garbage collect it
}

bool Screen::isAlive() { 
    return this->alive; 
}
