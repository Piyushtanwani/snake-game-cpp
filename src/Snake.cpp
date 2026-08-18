#include "Snake.h"
#include <cstddef>

Snake::Snake(int width, int height) : maxX(width), maxY(height), grown(false) {
    Segment head = { width / 2, height / 2 };
    body.push_back(head);
    
    for (int i = 1; i < 3; i++) {
        Segment segment = { head.x - i, head.y };
        body.push_back(segment);
    }
    
    dir = RIGHT;
}

void Snake::ChangeDirection(Direction newDir) {
    if ((dir == LEFT && newDir != RIGHT) ||
        (dir == RIGHT && newDir != LEFT) ||
        (dir == UP && newDir != DOWN) ||
        (dir == DOWN && newDir != UP)) {
        dir = newDir;
    }
}

void Snake::Move() {
    Segment newHead = body[0];
    
    switch (dir) {
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case STOP: break;
    }
    
    body.insert(body.begin(), newHead);
    
    if (!grown) {
        body.pop_back();
    } else {
        grown = false;
    }
}

void Snake::Grow() {
    grown = true;
}

bool Snake::CheckSelfCollision() const {
    for (std::size_t i = 1; i < body.size(); i++) {
        if (body[0].x == body[i].x && body[0].y == body[i].y) {
            return true;
        }
    }
    return false;
}

bool Snake::IsBody(int x, int y) const {
    for (std::size_t i = 1; i < body.size(); i++) {
        if (body[i].x == x && body[i].y == y) {
            return true;
        }
    }
    return false;
}

void Snake::SetPosition(int x, int y) {
    if (body.empty()) return;
    
    int dx = x - body[0].x;
    int dy = y - body[0].y;
    
    for (auto& segment : body) {
        segment.x += dx;
        segment.y += dy;
    }
}