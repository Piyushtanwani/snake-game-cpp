#include "Game.h"
#include <cstdlib>
#include <ctime>

// Pre-warm random number generator at program start
namespace {
    class RandomInitializer {
    public:
        RandomInitializer() {
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            // Pre-generate a few random numbers to warm up
            for (int i = 0; i < 10; i++) {
                std::rand();
            }
        }
    };
    
    RandomInitializer randomInit;
}

int main() {
    Game game;
    game.Run();
    return 0;
}