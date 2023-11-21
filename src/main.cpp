#include "main.hpp"

#if TEST == 1
    #include "test/test.hpp"
    
    int main() {
        return testMain();
    }
#else
    #include <stdlib.h>
    #include "graphics/Launcher.hpp"
    
    int main() {
        Launcher launcher{};
        launcher.run();
        return EXIT_SUCCESS;
    }
#endif