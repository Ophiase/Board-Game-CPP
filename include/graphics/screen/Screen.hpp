#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Container.hpp"

class Screen : public Container {
private:
    bool alive{true};
    std::string title;
public:
    Screen(Launcher *launcher, std::string title="Unknown");
    bool isAlive();
    virtual ~Screen() = default;
};