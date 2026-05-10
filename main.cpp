#include "Desk.h"
#include <iostream>

int main() {
    try {
        Desk reception;
        reception.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
