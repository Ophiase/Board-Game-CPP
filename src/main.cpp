#include "main.hpp"

#if TEST == 1
    #include "test/test.hpp"
    
    int main() {
        return testMain();
    }
#else
    #include <stdlib.h>
    #include "utils/Cli.hpp"
    #include "graphics/Launcher.hpp"

    int main(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i)
            if (std::string(argv[i]) == "-s" ||
                std::string(argv[i]) == "-silence")
                Cli::muted = true;
            
        Launcher launcher{};
        launcher.run();
        return EXIT_SUCCESS;
    }
#endif