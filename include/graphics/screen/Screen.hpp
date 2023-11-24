#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Container.hpp"

class Screen : public Container {
private:
    bool alive{true};
public:
    Screen(Launcher *launcher) : Container{launcher} {};
    virtual std::string getTitle();
    bool isAlive();
    
    virtual ~Screen() = default;
};