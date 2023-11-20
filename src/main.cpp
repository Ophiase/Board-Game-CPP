#include "main.hpp"

#include <iostream>
#include <stdlib.h>

#include <SFML/Graphics.hpp>

#include "graphics/Launcher.hpp"
#include "utils/debug.hpp"

int main() {
    #if TEST == 1
        std::cout << "Test !" << std::endl;
    #else
        Launcher launcher{};
        launcher.run();
    #endif

    return EXIT_SUCCESS;
}
