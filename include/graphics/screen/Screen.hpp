#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Container.hpp"

class Screen : public Container {
private:
    bool alive{true};
public:
    Screen(sf::RenderWindow & window) : Container{window} {};

    virtual std::string getTitle();
    bool isAlive();
    
    virtual ~Screen() = default;
};