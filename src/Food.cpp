#include "Food.h"

Food::Food(int width, int height) : maxX(width), maxY(height) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Food::Generate(const Snake& snake) {
    bool onSnake;
    
    do {
        onSnake = false;
        x = rand() % maxX;
        y = rand() % maxY;
        
        // Check if food would spawn on snake head
        if (x == snake.GetHeadX() && y == snake.GetHeadY()) {
            onSnake = true;
            continue;
        }
        
        // Check if food would spawn on snake body
        if (snake.IsBody(x, y)) {
            onSnake = true;
            continue;
        }
        
    } while (onSnake);
}