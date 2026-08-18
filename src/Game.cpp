#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>

using namespace std;

Game::Game() : food(WIDTH, HEIGHT), gameOver(false), paused(false), foodCount(0), loserPlayer(0) {
    InitializePlayers();
    GenerateObstacles();
    food.GenerateWithObstacles(snakes, obstacles);
}

void Game::InitializePlayers() {
    snakes.clear();
    scores.clear();
    
    // Player 1
    Snake snake1(WIDTH, HEIGHT);
    snake1.SetPosition(WIDTH/3, HEIGHT/2);
    snakes.push_back(snake1);
    scores.push_back(0);
    
    // Player 2
    Snake snake2(WIDTH, HEIGHT);
    snake2.SetPosition(WIDTH*2/3, HEIGHT/2);
    snakes.push_back(snake2);
    scores.push_back(0);
}

void Game::GenerateObstacles() {
    obstacles.clear();
    
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Define safe zone in the center where snake starts
    int safeZoneX = WIDTH / 2;
    int safeZoneY = HEIGHT / 2;
    int safeRadius = 4;
    
    // Pattern 1: Vertical walls
    for (int i = 0; i < 3; i++) {
        int wallX = 5 + (rand() % (WIDTH - 10));
        int wallY = 3 + (rand() % (HEIGHT - 6));
        int wallHeight = 3 + (rand() % 4);
        
        // Check if wall is too close to safe zone
        if (abs(wallX - safeZoneX) < safeRadius) continue;
        
        for (int y = wallY; y < wallY + wallHeight && y < HEIGHT - 1; y++) {
            // Don't place on borders and ensure it's not in safe zone
            if (y > 1 && y < HEIGHT - 1 && abs(y - safeZoneY) >= safeRadius) {
                obstacles.push_back({wallX, y});
            }
        }
    }
    
    // Pattern 2: Horizontal walls
    for (int i = 0; i < 3; i++) {
        int wallY = 5 + (rand() % (HEIGHT - 10));
        int wallX = 3 + (rand() % (WIDTH - 6));
        int wallWidth = 4 + (rand() % 5);
        
        // Check if wall is too close to safe zone
        if (abs(wallY - safeZoneY) < safeRadius) continue;
        
        for (int x = wallX; x < wallX + wallWidth && x < WIDTH - 1; x++) {
            // Don't place on borders and ensure it's not in safe zone
            if (x > 1 && x < WIDTH - 1 && abs(x - safeZoneX) >= safeRadius) {
                obstacles.push_back({x, wallY});
            }
        }
    }
    
    // Pattern 3: L-shaped corners
    for (int i = 0; i < 2; i++) {
        int cornerX = 3 + (rand() % (WIDTH - 8));
        int cornerY = 3 + (rand() % (HEIGHT - 8));
        
        // Check if corner is too close to safe zone
        if (abs(cornerX - safeZoneX) < safeRadius + 2 || abs(cornerY - safeZoneY) < safeRadius + 2) continue;
        
        // Create L-shape (3 blocks horizontal, 3 blocks vertical)
        for (int x = cornerX; x < cornerX + 3 && x < WIDTH - 1; x++) {
            obstacles.push_back({x, cornerY});
        }
        for (int y = cornerY; y < cornerY + 3 && y < HEIGHT - 1; y++) {
            obstacles.push_back({cornerX, y});
        }
    }
    
    // Pattern 4: Plus signs (+)
    for (int i = 0; i < 2; i++) {
        int centerX = 6 + (rand() % (WIDTH - 12));
        int centerY = 6 + (rand() % (HEIGHT - 12));
        
        // Check if plus is too close to safe zone
        if (abs(centerX - safeZoneX) < safeRadius + 1 || abs(centerY - safeZoneY) < safeRadius + 1) continue;
        
        // Create plus shape
        obstacles.push_back({centerX, centerY});
        obstacles.push_back({centerX - 1, centerY});
        obstacles.push_back({centerX + 1, centerY});
        obstacles.push_back({centerX, centerY - 1});
        obstacles.push_back({centerX, centerY + 1});
    }
    
    // Pattern 5: Border obstacles (partial walls near edges)
    // Top border obstacles
    for (int x = 3; x < WIDTH - 3; x += 4) {
        if (abs(x - safeZoneX) > safeRadius) {
            obstacles.push_back({x, 2});
        }
    }
    
    // Bottom border obstacles
    for (int x = 4; x < WIDTH - 4; x += 4) {
        if (abs(x - safeZoneX) > safeRadius) {
            obstacles.push_back({x, HEIGHT - 3});
        }
    }
    
    // Left border obstacles
    for (int y = 3; y < HEIGHT - 3; y += 4) {
        if (abs(y - safeZoneY) > safeRadius) {
            obstacles.push_back({2, y});
        }
    }
    
    // Right border obstacles
    for (int y = 4; y < HEIGHT - 4; y += 4) {
        if (abs(y - safeZoneY) > safeRadius) {
            obstacles.push_back({WIDTH - 3, y});
        }
    }
    
    // Remove any duplicates
    sort(obstacles.begin(), obstacles.end());
    obstacles.erase(unique(obstacles.begin(), obstacles.end()), obstacles.end());
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
    cout << "  3 Avoid obstacles (X) - random in each game" << endl;
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  4 Larger 30x20 game area" << endl;
    
    coord.Y += 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "  5 Each food gives 10 points" << endl;
    
    coord.Y += 3;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "Press any key to return to menu..." << endl;
    
    _getch();
}

void Game::Reset() {
    system("cls");
    InitializePlayers();
    GenerateObstacles();
    food.GenerateWithObstacles(snakes, obstacles);
    scores[0] = 0;
    scores[1] = 0;
    gameOver = false;
    paused = false;
    loserPlayer = 0;
    foodCount = 0; // Reset food counter
}

void Game::Draw() {
    // Move cursor to top-left for smooth rendering
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
    // Draw top border (double width)
    colorManager.SetColor(ColorManager::WALL);
    for (int i = 0; i < (WIDTH * 2) + 4; i++)
        cout << "#";
    cout << endl;
    
    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        // Left border
        colorManager.SetColor(ColorManager::WALL);
        cout << "##";
        
        for (int x = 0; x < WIDTH; x++) {
            bool isObstacle = false;
            
            // Check if this position is an obstacle
            for (const auto& obstacle : obstacles) {
                if (x == obstacle.first && y == obstacle.second) {
                    colorManager.SetColor(ColorManager::WALL);
                    cout << "\xDB\xDB";  // Double width block obstacles
                    isObstacle = true;
                    break;
                }
            }
            
            if (isObstacle) {
                continue;
            }
            
            // Reset to wall color for empty spaces
            colorManager.SetColor(ColorManager::WALL);
            
            if (x == snakes[0].GetHeadX() && y == snakes[0].GetHeadY()) {
                colorManager.SetColor(ColorManager::SNAKE_HEAD);
                cout << "\xDB\xDB";  // Double width block head
            }
            else if (x == snakes[1].GetHeadX() && y == snakes[1].GetHeadY()) {
                colorManager.SetColor(ColorManager::SNAKE_HEAD);
                cout << "\xDB\xDB";
            }
            else if (snakes[0].IsBody(x, y) || snakes[1].IsBody(x, y)) {
                colorManager.SetColor(ColorManager::SNAKE_BODY);
                cout << "\xDB\xDB";  // Double width block body
            }
            else if (x == food.GetX() && y == food.GetY() && food.IsActive()) {
                // Use food's own color and character
                colorManager.SetColor(food.GetColor());
                cout << food.GetDisplayChar() << food.GetDisplayChar(); // Double width
                colorManager.SetColor(ColorManager::WALL);
            }
            else {
                cout << "  ";  // Double width space
            }
        }
        
        // Right border
        colorManager.SetColor(ColorManager::WALL);
        cout << "##" << endl;
    }
    
    // Draw bottom border (double width)
    colorManager.SetColor(ColorManager::WALL);
    for (int i = 0; i < (WIDTH * 2) + 4; i++)
        cout << "#";
    cout << endl;
    
    // Reset color for text
    colorManager.ResetColor();
    
    // Score with color - Bright White
    colorManager.SetColor(ColorManager::SCORE);
    cout << "  P1 Score: " << scores[0] << " | P2 Score: " << scores[1] << endl;
    
    // Status line with colors
    if (paused) {
        colorManager.SetColor(ColorManager::MENU_TITLE);
        PauseGame();
    } else {
        if (highScore.IsHighScore(scores[0])) {
            colorManager.SetColor(ColorManager::HIGH_SCORE);
            cout << "High Score Potential! | Controls: WASD/Arrows, P=Pause, X=Exit" << endl;
        } else {
            colorManager.SetColor(ColorManager::SCORE);
            cout << "Controls: WASD/Arrows, P=Pause, X=Exit" << endl;
        }
    }
    
    // Clear any remaining lines to prevent ghost text
    cout << "                                                                  " << endl;
    cout << "                                                                  " << endl;
    
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
                case 72: snakes[0].ChangeDirection(UP); break;
                case 80: snakes[0].ChangeDirection(DOWN); break;
                case 75: snakes[0].ChangeDirection(LEFT); break;
                case 77: snakes[0].ChangeDirection(RIGHT); break;
            }
        } else {
            switch (key) {
                case 'w': case 'W': snakes[1].ChangeDirection(UP); break;
                case 's': case 'S': snakes[1].ChangeDirection(DOWN); break;
                case 'a': case 'A': snakes[1].ChangeDirection(LEFT); break;
                case 'd': case 'D': snakes[1].ChangeDirection(RIGHT); break;
                case 'p': case 'P': PauseGame(); break;
                case 'x': case 'X': gameOver = true; break;
            }
        }
    }
}

void Game::Logic() {
    if (paused) return;
    
    for (auto& snake : snakes) {
        snake.Move();
    }
    
    std::vector<bool> lost(snakes.size(), false);
    
    for (size_t i = 0; i < snakes.size(); ++i) {
        int headX = snakes[i].GetHeadX();
        int headY = snakes[i].GetHeadY();
        
        // Check wall collisions
        if (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT) lost[i] = true;
        
        // Check obstacle collisions
        for (const auto& obstacle : obstacles) {
            if (headX == obstacle.first && headY == obstacle.second) lost[i] = true;
        }
        
        // Check self collision
        if (snakes[i].CheckSelfCollision()) lost[i] = true;
    }
    
    // Check collision with each other
    int headX = snakes[0].GetHeadX();
    int headY = snakes[0].GetHeadY();
    int head2X = snakes[1].GetHeadX();
    int head2Y = snakes[1].GetHeadY();
    
    if (headX == head2X && headY == head2Y) {
        lost[0] = true;
        lost[1] = true;
    } else {
        if (snakes[1].IsBody(headX, headY)) lost[0] = true;
        if (snakes[0].IsBody(head2X, head2Y)) lost[1] = true;
    }

    if (lost[0] && lost[1]) loserPlayer = 3;
    else if (lost[0]) loserPlayer = 1;
    else if (lost[1]) loserPlayer = 2;

    if (lost[0] || lost[1]) {
        EnterHighScore();
        gameOver = true;
        return;
    }
    
    // Check food collision
    bool ate[2] = {false, false};
    for (size_t i = 0; i < snakes.size(); ++i) {
        if (snakes[i].GetHeadX() == food.GetX() && snakes[i].GetHeadY() == food.GetY() && food.IsActive()) {
            ate[i] = true;
        }
    }

    if (ate[0] || ate[1]) {
        // Play sound based on food type
        if (food.GetType() == Food::GOLDEN) {
            Beep(1000, 200); // Special sound for golden food
            Beep(1200, 200);
        } else {
            soundManager.PlayEatSound(); // Regular sound
        }
        
        for (size_t i = 0; i < snakes.size(); ++i) {
            if (ate[i]) {
                scores[i] += food.GetPoints();
                snakes[i].Grow();
            }
        }
        
        // Increment food counter
        foodCount++;
        
        if (foodCount % 5 == 0) {
            food.GenerateSpecialFood(snakes);
        } else {
            food.GenerateWithObstacles(snakes, obstacles);
        }
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
    int maxScore = std::max(scores[0], scores[1]);
    if (highScore.IsHighScore(maxScore)) {
        system("cls");
        
        colorManager.SetColor(ColorManager::HIGH_SCORE);
        cout << "*** NEW HIGH SCORE! ***" << endl;
        colorManager.ResetColor();
        
        cout << "High Score: " << maxScore << endl;
        cout << endl;
        cout << "Enter your name (max 10 letters, letters only): ";
        
        string name = "";
        char ch;
        bool nameComplete = false;
        
        while (!nameComplete) {
            ch = _getch(); // Get character without echo
            
            if (ch == 13 && name.length() > 0) { // Enter key
                nameComplete = true;
            }
            else if (ch == 8 && name.length() > 0) { // Backspace
                name.pop_back();
                cout << " "; // Erase the character from console
            }
            else if (isalpha(ch) && name.length() < 10) { // Only letters and max 10 chars
                name += toupper(ch);
                cout << static_cast<char>(toupper(ch)); // Echo the character
            }
        }
        
        cout << endl;
        cout << "Thank you, " << name << "!" << endl;
        Sleep(1000);
        
        highScore.AddScore(name, maxScore);
        
        system("cls");
    }
}


void Game::GameOverScreen() {
    system("cls");
    
    int maxScore = std::max(scores[0], scores[1]);
    bool isNewHighScore = highScore.IsHighScore(maxScore);
    
    if (isNewHighScore) {
        colorManager.SetColor(ColorManager::HIGH_SCORE);
        cout << "##############################" << endl;
        cout << "#       NEW HIGH SCORE!     #" << endl;
        cout << "##############################" << endl;
        
        colorManager.SetColor(ColorManager::HIGH_SCORE);
        cout << "  P1 Score: " << scores[0] << " | P2 Score: " << scores[1] << endl;
        if (loserPlayer == 1) cout << "     Player 1 lost!" << endl;
        else if (loserPlayer == 2) cout << "     Player 2 lost!" << endl;
        else if (loserPlayer == 3) cout << "     Both players lost!" << endl;
        
        colorManager.SetColor(ColorManager::HIGH_SCORE);
        cout << "##############################" << endl;
    } else {
        colorManager.SetColor(ColorManager::GAME_OVER);
        cout << "########################" << endl;
        cout << "#      GAME OVER      #" << endl;
        cout << "########################" << endl;
        
        colorManager.SetColor(ColorManager::SCORE);
        cout << "  P1 Score: " << scores[0] << " | P2 Score: " << scores[1] << endl;
        if (loserPlayer == 1) cout << "     Player 1 lost!" << endl;
        else if (loserPlayer == 2) cout << "     Player 2 lost!" << endl;
        else if (loserPlayer == 3) cout << "     Both players lost!" << endl;
        
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
    
    gameOver = false;
    paused = false;
    
    Draw();
    
    // Simple game loop
    while (!gameOver) {
        Input();
        Logic();
        Draw();
        Sleep(80);
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