#include <iostream>

#include "src/Yasg.h"

using namespace yasg;

int main(int argc, char **argv) {
    try {
        
        Yasg yasg = Yasg();
        yasg.run();
        
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
