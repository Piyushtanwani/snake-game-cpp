#ifndef FOOD_H
#define FOOD_H

#include "Snake.h"
#include <vector>
#include <utility>

class Food {
private:
    int x, y;
    int maxX, maxY;
    
public:
    Food(int width, int height);
    void Generate(const Snake& snake);
    void GenerateWithObstacles(const Snake& snake, const std::vector<std::pair<int, int>>& obstacles);
    
    // Getters
    int GetX() const { return x; }
    int GetY() const { return y; }
};

#endif