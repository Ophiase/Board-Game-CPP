#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Container.hpp"

class Screen : public Container {
private:
    bool alive = true;
public:
    virtual std::string getTitle();
    bool isAlive();
    
    virtual ~Screen() = default;
};