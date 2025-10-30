#include "Food.h"
#include <cstdlib>
#include <ctime>

// Initialize random seed only once
namespace {
    bool randomInitialized = false;
    void InitializeRandom() {
        if (!randomInitialized) {
            srand(static_cast<unsigned int>(time(nullptr)));
            randomInitialized = true;
        }
    }
}

Food::Food(int width, int height) : maxX(width), maxY(height) {
    // Don't initialize random here - lazy initialization
}

void Food::Generate(const Snake& snake) {
    if (!randomInitialized) {
        InitializeRandom();
    }
    
    bool onSnake;
    
    do {
        onSnake = false;
        x = rand() % maxX;
        y = rand() % maxY;
        
        if (x == snake.GetHeadX() && y == snake.GetHeadY()) {
            onSnake = true;
            continue;
        }
        
        if (snake.IsBody(x, y)) {
            onSnake = true;
        }
        
    } while (onSnake);
}

void Food::GenerateWithObstacles(const Snake& snake, const std::vector<std::pair<int, int>>& obstacles) {
    if (!randomInitialized) {
        InitializeRandom();
    }
    
    bool invalidPosition;
    
    do {
        invalidPosition = false;
        x = rand() % maxX;
        y = rand() % maxY;
        
        // Check if position is on snake head
        if (x == snake.GetHeadX() && y == snake.GetHeadY()) {
            invalidPosition = true;
            continue;
        }
        
        // Check if position is on snake body
        if (snake.IsBody(x, y)) {
            invalidPosition = true;
            continue;
        }
        
        // Check if position is on any obstacle
        for (const auto& obstacle : obstacles) {
            if (x == obstacle.first && y == obstacle.second) {
                invalidPosition = true;
                break;
            }
        }
        
    } while (invalidPosition);
}