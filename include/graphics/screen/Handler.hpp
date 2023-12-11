#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Handler {
    public:
        /*
            Returns true if the Event was consummed.
            Usecase :
                When a button change the interface, it's necessary
                to ensure that the same event that was used by the
                button to launch the interface is not directly used
                by the activated interface.
        */
        virtual bool handleEvent(sf::Event) { return false; };
};