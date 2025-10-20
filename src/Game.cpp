#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>

using namespace std;

Game::Game() : snake(WIDTH, HEIGHT), food(WIDTH, HEIGHT), score(0), gameOver(false) {
    food.Generate(snake);
}

void Game::Draw() {
    // Move cursor to top-left instead of clearing screen
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Draw left border
            if (x == 0)
                cout << "#";
            
            // Draw snake head
            if (x == snake.GetHeadX() && y == snake.GetHeadY())
                cout << "O";
            // Draw snake body
            else if (snake.IsBody(x, y))
                cout << "o";
            // Draw food
            else if (x == food.GetX() && y == food.GetY())
                cout << "*";
            else
                cout << " ";
            
            // Draw right border
            if (x == WIDTH - 1)
                cout << "#";
        }
        cout << endl;
    }
    
    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    
    cout << "Score: " << score << "   " << endl;
    cout << "Controls: WASD or Arrow Keys to move, X to exit    " << endl;
}

void Game::Input() {
    // Clear any buffered keys
    while (_kbhit()) {
        int key = _getch();
        
        // Handle arrow keys (they produce two codes: 224/0 + arrow code)
        if (key == 224 || key == 0) {
            // Arrow key pressed, get the actual arrow key code
            if (_kbhit()) {
                int arrowKey = _getch();
                switch (arrowKey) {
                    case 72: // Up arrow
                        snake.ChangeDirection(UP);
                        break;
                    case 80: // Down arrow
                        snake.ChangeDirection(DOWN);
                        break;
                    case 75: // Left arrow
                        snake.ChangeDirection(LEFT);
                        break;
                    case 77: // Right arrow
                        snake.ChangeDirection(RIGHT);
                        break;
                }
            }
        }
        // Handle WASD keys and exit
        else {
            switch (key) {
                case 'w': case 'W':
                    snake.ChangeDirection(UP);
                    break;
                case 's': case 'S':
                    snake.ChangeDirection(DOWN);
                    break;
                case 'a': case 'A':
                    snake.ChangeDirection(LEFT);
                    break;
                case 'd': case 'D':
                    snake.ChangeDirection(RIGHT);
                    break;
                case 'x': case 'X':
                    gameOver = true;
                    break;
            }
        }
    }
}

void Game::Logic() {
    snake.Move();
    
    // Check collision with walls
    int headX = snake.GetHeadX();
    int headY = snake.GetHeadY();
    
    if (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT) {
        gameOver = true;
        return;
    }
    
    // Check collision with self
    if (snake.CheckSelfCollision()) {
        gameOver = true;
        return;
    }
    
    // Check if snake ate food
    if (headX == food.GetX() && headY == food.GetY()) {
        snake.Grow();
        score += 10;
        food.Generate(snake);
    }
}

void Game::GameOverScreen() {
    system("cls");  // We can use cls here since it's only once
    cout << "########################" << endl;
    cout << "#      GAME OVER      #" << endl;
    cout << "########################" << endl;
    cout << "     Final Score: " << score << endl;
    cout << "########################" << endl;
    cout << "Press any key to exit..." << endl;
    _getch();
}

void Game::Run() {
    // Hide cursor for better appearance
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(150);
    }
    
    // Restore cursor
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    GameOverScreen();
}