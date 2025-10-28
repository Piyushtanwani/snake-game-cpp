🐍 Snake Game
A classic Snake game implemented in C++ with console graphics, featuring colorful visuals, sound effects, and high score tracking.

📋 Table of Contents
Features

Project Structure

Installation

How to Play

Controls

Game Rules

Compilation

Class Documentation

Future Enhancements

✨ Features
🎮 Classic Snake Gameplay - Move the snake, eat food, and grow longer

🎨 Colorful Graphics - Different colors for snake head, body, food, and UI elements

🔊 Sound Effects - Eating sounds and menu selection sounds

📊 High Score System - Persistent high score tracking with file storage

🏆 High Score Management - Update existing user scores instead of duplicates

🎯 Smooth Controls - Responsive keyboard controls with both WASD and arrow keys

⏸️ Pause Functionality - Pause and resume the game anytime

📱 Interactive Menus - Beautiful menu system with navigation

❓ Help Screen - Comprehensive game instructions


📁 Project Structure
text
Snake Game/
├── src/
│   ├── main.cpp              # Entry point with random initialization
│   ├── Game.h & Game.cpp     # Main game controller and menu system
│   ├── Snake.h & Snake.cpp   # Snake movement and collision logic
│   ├── Food.h & Food.cpp     # Food generation and placement
│   ├── HighScore.h & HighScore.cpp # High score management
│   ├── ColorManager.h & ColorManager.cpp # Console color handling
│   └── SoundManager.h & SoundManager.cpp # Sound effects
├── highscores.txt            # Auto-generated high score storage
├── .gitignore               # Git ignore file
└── README.md               # This file
🛠️ Installation
Prerequisites
C++ Compiler (MinGW-w64 recommended for Windows)

Windows OS (uses Windows API for console functions)

Quick Start
Clone or download the project

Navigate to the project directory

Compile the game:

bash
g++ -std=c++11 src/*.cpp -o snake_game.exe -static
Run the game:

bash
snake_game.exe
🎮 How to Play
Start the game and navigate through menus using W/S or arrow keys

Control the snake to eat food (*) and grow longer

Avoid collisions with walls and your own body

Score points - each food gives 10 points

Compete for high scores and try to beat your personal best!

⌨️ Controls
Key	Action
W / ↑	Move Up
S / ↓	Move Down
A / ←	Move Left
D / →	Move Right
P	Pause/Resume Game
X	Exit Game
ENTER	Select Menu Option
📜 Game Rules
Eating Food: Each food item (*) increases your score by 10 points and makes the snake grow longer

Collisions: Game ends if you hit the walls or your own body

Movement: Snake moves continuously in the current direction

High Scores: Top 5 scores are saved and displayed

🔧 Compilation
Windows (MinGW)
bash
g++ -std=c++11 src/*.cpp -o snake_game.exe -static
Compilation Flags
-std=c++11: Use C++11 standard

-static: Static linking for portability

-o snake_game.exe: Output executable name

📚 Class Documentation
Game Class
Purpose: Main game controller

Features: Game loop, menu system, input handling, rendering

Key Methods: Run(), ShowMainMenu(), StartGame(), Logic()

Snake Class
Purpose: Snake behavior and movement

Features: Direction control, growth, collision detection

Key Methods: Move(), ChangeDirection(), Grow(), CheckSelfCollision()

Food Class
Purpose: Food generation and placement

Features: Random spawning, collision avoidance with snake

Key Methods: Generate(), position getters

HighScore Class
Purpose: High score management

Features: File I/O, score sorting, duplicate user handling

Key Methods: AddScore(), IsHighScore(), DisplayScores()

ColorManager Class
Purpose: Console color management

Features: Color constants, text attribute setting

Key Methods: SetColor(), ResetColor()

SoundManager Class
Purpose: Sound effects

Features: Beep-based sounds for game events

Key Methods: PlayEatSound(), PlayMenuSelectSound()

🚀 Future Enhancements
Potential features for future versions:

Difficulty levels (Easy, Medium, Hard)

Special food types with different effects

Obstacles and walls within the game area

Level progression system

Power-ups (speed boost, shield, etc.)

Game statistics and achievements

Customizable controls

Background music

🐛 Troubleshooting
Common Issues
"Permission denied" during compilation: Close any running instances of the game

No sound: Ensure your system volume is up and speakers are working

Colors not displaying: The game requires a terminal that supports ANSI colors

File Dependencies
highscores.txt: Auto-created on first run, stores high scores

All source files in src/ directory

👨‍💻 Development
This project was developed as a college programming assignment to demonstrate:

Object-Oriented Programming principles

C++ file I/O operations

Windows API integration

Game development concepts

Code organization and modular design

📄 License
This project is for educational purposes as part of a college programming course.

Enjoy playing! 🐍🎮