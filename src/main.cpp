#include "main.hpp"

#include <iostream>
#include <stdlib.h>

#include "graphics/Launcher.hpp"
#include "utils/debug.hpp"

int main() {
    #ifndef TEST
        Launcher launcher{};
    #else
        std::cout << "Test !" << std::endl;
    #endif

    return EXIT_SUCCESS;
}