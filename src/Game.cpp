#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <algorithm>

using namespace std;

Game::Game() : snake(WIDTH, HEIGHT), food(WIDTH, HEIGHT), score(0), gameOver(false), paused(false) {
    food.Generate(snake);
}

void Game::Reset() {
    // Clear the console completely before resetting
    system("cls");
    
    // Reset the existing objects
    snake = Snake(WIDTH, HEIGHT);
    food.Generate(snake);
    score = 0;
    gameOver = false;
    paused = false;
}

void Game::Draw() {
    // Use a single string buffer for better performance
    static string buffer;
    buffer.clear();
    
    // Top border
    buffer.append(WIDTH + 2, '#');
    buffer += '\n';
    
    // Game area
    for (int y = 0; y < HEIGHT; y++) {
        buffer += '#'; // Left border
        
        for (int x = 0; x < WIDTH; x++) {
            if (x == snake.GetHeadX() && y == snake.GetHeadY())
                buffer += 'O';
            else if (snake.IsBody(x, y))
                buffer += 'o';
            else if (x == food.GetX() && y == food.GetY())
                buffer += '*';
            else
                buffer += ' ';
        }
        
        buffer += "#\n"; // Right border
    }
    
    // Bottom border
    buffer.append(WIDTH + 2, '#');
    buffer += '\n';
    
    // Score info
    buffer += "Score: " + to_string(score) + "               \n";
    
    if (paused) {
        buffer += "*** GAME PAUSED *** Press P to resume\n";
    } else {
        if (highScore.IsHighScore(score)) {
            buffer += "High Score Potential!        \n";
        } else {
            buffer += "Controls: WASD/Arrows, P=Pause, X=Exit\n";
        }
    }
    buffer += "                              \n";
    
    // Move cursor and draw everything at once
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << buffer;
}

void Game::Input() {
    if (_kbhit()) {
        int key = _getch();
        
        // If game is paused, only check for resume key (P)
        if (paused) {
            if (key == 'p' || key == 'P') {
                paused = false;
            }
            return;
        }
        
        // Handle arrow keys (they produce two codes: 224 + arrow code)
        if (key == 224) {
            // Arrow key pressed, get the actual arrow key code
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
        // Handle WASD keys and other controls
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
                case 'p': case 'P':  // Pause the game
                    PauseGame();
                    break;
                case 'x': case 'X':
                    gameOver = true;
                    break;
            }
        }
    }
}

void Game::Logic() {
    // Don't update game logic if paused
    if (paused) {
        return;
    }
    
    snake.Move();
    
    // Check collision with walls
    int headX = snake.GetHeadX();
    int headY = snake.GetHeadY();
    
    if (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT) {
        EnterHighScore();
        gameOver = true;
        return;
    }
    
    // Check collision with self
    if (snake.CheckSelfCollision()) {
        EnterHighScore();
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

void Game::PauseGame() {
    paused = true;
    
    // Create a simple pause screen
    COORD coord = {0, HEIGHT + 4};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
    cout << "*** GAME PAUSED ***" << endl;
    cout << "Press P to resume" << endl;
}

void Game::EnterHighScore() {
    if (highScore.IsHighScore(score)) {
        system("cls");
        cout << "###############################" << endl;
        cout << "#      NEW HIGH SCORE!       #" << endl;
        cout << "###############################" << endl;
        cout << "     Your Score: " << score << endl;
        cout << endl;
        cout << "Enter your name (3 letters): ";
        
        string name;
        cin >> name;
        
        // Limit to 3 characters
        if (name.length() > 3) {
            name = name.substr(0, 3);
        }
        
        // Convert to uppercase
        for (char& c : name) {
            c = toupper(c);
        }
        
        // Default if empty
        if (name.empty()) {
            name = "AAA";
        }
        
        highScore.AddScore(name, score);
    }
}

void Game::GameOverScreen() {
    system("cls");
    cout << "########################" << endl;
    cout << "#      GAME OVER      #" << endl;
    cout << "########################" << endl;
    cout << "     Final Score: " << score << endl;
    
    // Check if it's a high score
    if (highScore.IsHighScore(score)) {
        cout << "   NEW HIGH SCORE!   " << endl;
    }
    
    cout << "########################" << endl;
    cout << endl;
    cout << "1. View High Scores" << endl;
    cout << "2. Play Again" << endl;
    cout << "3. Exit" << endl;
    cout << endl;
    cout << "Choose option: ";
    
    char choice;
    cin >> choice;
    
    switch (choice) {
        case '1':
            highScore.DisplayScores();
            // After viewing scores, show this menu again
            GameOverScreen();
            break;
        case '2':
            // Reset game and start new game
            Reset();
            Run();  // Restart the game loop
            break;
        case '3':
        default:
            // Exit the game
            break;
    }
}

void Game::Run() {
    // Clear screen at the start
    system("cls");
    
    // Hide cursor for better appearance
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    // Draw initial frame immediately
    Draw();
    
    // Main game loop
    while (!gameOver) {
        Input();
        Logic();
        Draw();
        Sleep(150);
    }
    
    // Restore cursor
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    GameOverScreen();
}