#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>



#include "EditorApp.h"

int main() {
    EditorApp app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
