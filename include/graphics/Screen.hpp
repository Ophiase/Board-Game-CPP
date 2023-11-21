#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Screen {
private:
    bool alive = true;
public:
    virtual std::string getTitle();
    virtual void draw(sf::RenderWindow& window) = 0;
    
    /*
        Returns nullptr or a sucessor Screen.
    */
    virtual Screen *handleEvent(sf::Event event) = 0;
    virtual ~Screen() = default;

    bool isAlive();
};