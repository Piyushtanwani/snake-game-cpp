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

void Game::DrawMenuBorder() {
    system("cls");
    
    // Draw a fancy border for the menu
    string topBottom = "===================================================";
    string side = "|                                                 |";
    
    cout << topBottom << endl;
    for (int i = 0; i < 25; i++) {
        cout << side << endl;
    }
    cout << topBottom << endl;
}

void Game::ShowMainMenu() {
    static int selectedOption = 0;
    const int TOTAL_OPTIONS = 4;
    string options[TOTAL_OPTIONS] = {
        "Start Game",
        "View High Scores", 
        "Help",
        "Exit"
    };
    
    while (true) {
        DrawMenuBorder();
        
        COORD coord = {5, 3};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        // Title in yellow
        colorManager.SetColor(ColorManager::MENU_TITLE);
        cout << "           <<< SNAKE GAME >>>" << endl;
        colorManager.ResetColor();
        
        coord.Y += 4;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "Use W/S or Arrow Keys to navigate" << endl;
        
        coord.Y += 2;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        // Menu options with colors
        for (int i = 0; i < TOTAL_OPTIONS; i++) {
            if (i == selectedOption) {
                colorManager.SetColor(ColorManager::MENU_SELECTED);
                cout << "    > " << options[i] << " <" << endl;
            } else {
                colorManager.SetColor(ColorManager::MENU_NORMAL);
                cout << "      " << options[i] << "   " << endl;
            }
            coord.Y += 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        }
        
        colorManager.ResetColor();
        
        coord.Y += 2;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "Press ENTER to select" << endl;
        
        // Wait for input without rapid redrawing
        while (!_kbhit()) {
            Sleep(100);
        }
        
        int key = _getch();
        
        if (key == 224) {
            int arrowKey = _getch();
            if (arrowKey == 72) { // Up arrow
                selectedOption = (selectedOption - 1 + TOTAL_OPTIONS) % TOTAL_OPTIONS;
            } else if (arrowKey == 80) { // Down arrow
                selectedOption = (selectedOption + 1) % TOTAL_OPTIONS;
            }
        } else {
            switch (key) {
                case 'w': case 'W':
                    selectedOption = (selectedOption - 1 + TOTAL_OPTIONS) % TOTAL_OPTIONS;
                    break;
                case 's': case 'S':
                    selectedOption = (selectedOption + 1) % TOTAL_OPTIONS;
                    break;
                case 13: // Enter key
                    soundManager.PlayMenuSelectSound();
                    switch (selectedOption) {
                        case 0: return; // Start Game
                        case 1: 
                            highScore.DisplayScores();
                            break;
                        case 2: 
                            ShowHelpScreen();
                            break;
                        case 3: 
                            exit(0);
                            break;
                    }
                    break;
                case 'x': case 'X':
                    exit(0);
                    break;
            }
        }
    }
}

void Game::ShowHelpScreen() {
    DrawMenuBorder();
    
    COORD coord = {5, 3};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Title in yellow
    colorManager.SetColor(ColorManager::MENU_TITLE);
    cout << "          <<< HOW TO PLAY >>>          " << endl;
    colorManager.ResetColor();

    coord.Y += 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
    // Section headers in cyan
    colorManager.SetColor(ColorManager::MENU_SELECTED);
    cout << "CONTROLS:" << endl;
    colorManager.ResetColor();
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  W / Up Arrow   - Move Up" << endl;
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  S / Down Arrow - Move Down" << endl;

    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  A / Left Arrow - Move Left" << endl;

    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  D / Right Arrow - Move Right" << endl;

    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  P - Pause Game" << endl;
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  X - Exit Game" << endl;
    
    coord.Y += 3;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
    colorManager.SetColor(ColorManager::MENU_SELECTED);
    cout << "GAME RULES:" << endl;
    colorManager.ResetColor();
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  1 Eat food (*) to grow and earn points" << endl;
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  2 Avoid hitting walls or yourself" << endl;
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  3 Each food gives 10 points" << endl;
    
    coord.Y += 3;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "Press any key to return to menu..." << endl;
    
    _getch();
}

void Game::Reset() {
    system("cls");
    snake = Snake(WIDTH, HEIGHT);
    food.Generate(snake);
    score = 0;
    gameOver = false;
    paused = false;
}

void Game::Draw() {
    // Don't use static string buffer - draw directly with colors
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    cout << "          <<< HOW TO PLAY >>>          " << endl;

    coord.Y += 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "CONTROLS:" << endl;
    
    // Set wall color and draw top border
    colorManager.SetColor(ColorManager::WALL);
    for (int i = 0; i < WIDTH + 2; i++)
        cout << '#';
    cout << endl;
    
    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        // Left border
        colorManager.SetColor(ColorManager::WALL);
        cout << '#';
        
        for (int x = 0; x < WIDTH; x++) {
            if (x == snake.GetHeadX() && y == snake.GetHeadY()) {
                colorManager.SetColor(ColorManager::SNAKE_HEAD); // Bright Yellow
                cout << 'O';
                colorManager.SetColor(ColorManager::WALL);
            }
            else if (snake.IsBody(x, y)) {
                colorManager.SetColor(ColorManager::SNAKE_BODY); // Green
                cout << 'o';
                colorManager.SetColor(ColorManager::WALL);
            }
            else if (x == food.GetX() && y == food.GetY()) {
                colorManager.SetColor(ColorManager::FOOD); // Bright Red
                cout << '*';
                colorManager.SetColor(ColorManager::WALL);
            }
            else {
                cout << ' ';
            }
        }
        
        // Right border
        colorManager.SetColor(ColorManager::WALL);
        cout << '#' << endl;
    }
    
    // Draw bottom border
    colorManager.SetColor(ColorManager::WALL);
    for (int i = 0; i < WIDTH + 2; i++)
        cout << '#';
    cout << endl;
    
    // Reset color for text
    colorManager.ResetColor();
    
    // Score with color - Bright White
    colorManager.SetColor(ColorManager::SCORE);
    cout << "Score: " << score;
    int padding = 30 - (7 + to_string(score).length());
    if (padding > 0) cout << string(padding, ' ');
    cout << endl;
    
    // Status line with colors
    if (paused) {
        colorManager.SetColor(ColorManager::MENU_TITLE); // Bright Yellow
        cout << "*** GAME PAUSED *** \n\n  Press P to resume" << endl;
    } else {
        if (highScore.IsHighScore(score)) {
            colorManager.SetColor(ColorManager::HIGH_SCORE); // Bright Green
            cout << "High Score Potential! | Controls: WASD/Arrows, P=Pause, X=Exit" << endl;
        } else {
            colorManager.SetColor(ColorManager::SCORE); // Bright White
            cout << "Controls: WASD/Arrows, P=Pause, X=Exit" << endl;
        }
    }
    
    // Clear any remaining lines
    cout << "                              " << endl;
    
    // Reset to default color
    colorManager.ResetColor();
}

void Game::Input() {
    if (_kbhit()) {
        int key = _getch();
        
        if (paused) {
            if (key == 'p' || key == 'P') {
                paused = false;
            }
            return;
        }
        
        if (key == 224) {
            int arrowKey = _getch();
            switch (arrowKey) {
                case 72: snake.ChangeDirection(UP); break;
                case 80: snake.ChangeDirection(DOWN); break;
                case 75: snake.ChangeDirection(LEFT); break;
                case 77: snake.ChangeDirection(RIGHT); break;
            }
        } else {
            switch (key) {
                case 'w': case 'W': snake.ChangeDirection(UP); break;
                case 's': case 'S': snake.ChangeDirection(DOWN); break;
                case 'a': case 'A': snake.ChangeDirection(LEFT); break;
                case 'd': case 'D': snake.ChangeDirection(RIGHT); break;
                case 'p': case 'P': PauseGame(); break;
                case 'x': case 'X': gameOver = true; break;
            }
        }
    }
}

void Game::Logic() {
    if (paused) return;
    
    snake.Move();
    
    int headX = snake.GetHeadX();
    int headY = snake.GetHeadY();
    
    if (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT) {
        EnterHighScore();
        gameOver = true;
        return;
    }
    
    if (snake.CheckSelfCollision()) {
        EnterHighScore();
        gameOver = true;
        return;
    }
    
    if (headX == food.GetX() && headY == food.GetY()) {
        soundManager.PlayEatSound();  // Food eating sound
        snake.Grow();
        score += 10;
        food.Generate(snake);
    }
}

void Game::PauseGame() {
    paused = true;
    COORD coord = {0, HEIGHT + 4};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "*** GAME PAUSED ***" << endl;
    cout << "Press P to resume" << endl;
}

void Game::EnterHighScore() {
    if (highScore.IsHighScore(score)) {
        system("cls");
        
        // Show simple prompt without the fancy border
        colorManager.SetColor(ColorManager::HIGH_SCORE); // Bright Green
        cout << "*** NEW HIGH SCORE! ***" << endl;
        colorManager.ResetColor();
        
        cout << "Your Score: " << score << endl;
        cout << endl;
        cout << "Enter your name (3 letters): ";
        
        string name;
        cin >> name;
        
        if (name.length() > 3) name = name.substr(0, 3);
        for (char& c : name) c = toupper(c);
        if (name.empty()) name = "AAA";
        
        highScore.AddScore(name, score);
        
        // Clear the screen after name entry
        system("cls");
    }
}

void Game::GameOverScreen() {
    system("cls");
    
    bool isNewHighScore = highScore.IsHighScore(score);
    
    if (isNewHighScore) {
        // Show NEW HIGH SCORE in green instead of GAME OVER
        colorManager.SetColor(ColorManager::HIGH_SCORE); // Bright Green
        cout << "##############################" << endl;
        cout << "#       NEW HIGH SCORE!     #" << endl;
        cout << "##############################" << endl;
        
        // Score in GREEN for new high score
        colorManager.SetColor(ColorManager::HIGH_SCORE);
        cout << "        Final Score: " << score << endl;
        
        // Bottom border in green
        colorManager.SetColor(ColorManager::HIGH_SCORE);
        cout << "##############################" << endl;
    } else {
        // Show regular GAME OVER in red
        colorManager.SetColor(ColorManager::GAME_OVER); // Bright Red
        cout << "########################" << endl;
        cout << "#      GAME OVER      #" << endl;
        cout << "########################" << endl;
        
        // Score in White for regular game over
        colorManager.SetColor(ColorManager::SCORE);
        cout << "     Final Score: " << score << endl;
        
        colorManager.SetColor(ColorManager::GAME_OVER);
        cout << "########################" << endl;
    }
    
    colorManager.ResetColor();
    cout << endl;
    cout << "1. View High Scores" << endl;
    cout << "2. Play Again" << endl;
    cout << "3. Main Menu" << endl;
    cout << "4. Exit" << endl;
    cout << endl;
    cout << "Choose option: ";
    
    char choice;
    cin >> choice;
    
    switch (choice) {
        case '1':
            highScore.DisplayScores();
            GameOverScreen();
            break;
        case '2':
            Reset();
            StartGame();
            break;
        case '3':
            Reset();
            gameOver = true;
            return;
        case '4':
            exit(0);
            break;
        default:
            cout << "Invalid choice...!" << endl;
            Sleep(1000);
            GameOverScreen();
    }
}

// Start game without showing menu
void Game::StartGame() {
    // Clear screen and setup for gameplay
    system("cls");
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    gameOver = false; // Ensure game is not over
    paused = false;   // Ensure game is not paused
    
    Draw();
    
    while (!gameOver) {
        Input();
        Logic();
        Draw();
        Sleep(150);
    }
    
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    GameOverScreen();
}

void Game::Run() {
    while (true) {
        // Show main menu first
        ShowMainMenu();
        
        // After menu, start the game
        StartGame();
    }
}