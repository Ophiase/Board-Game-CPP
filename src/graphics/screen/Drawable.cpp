#include "graphics/screen/Drawable.hpp"
#include "graphics/Launcher.hpp"

Drawable& Drawable::operator=(const Drawable& other) {
            this->launcher = other.launcher; return *this; 
        };


sf::FloatRect Drawable::getScreen() const {
    return launcher->getScreen();
};
        
sf::FloatRect Drawable::getRenderZone() const {
    return launcher->getRenderZone();
}

sf::RenderWindow & Drawable::getRenderWindow() {
    return launcher->getRenderWindow();
}

sf::RenderWindow const & Drawable::getConstRenderWindow() const {
    return launcher->getRenderWindow();
}

Launcher *Drawable::getLauncher() {
    return this->launcher;
}