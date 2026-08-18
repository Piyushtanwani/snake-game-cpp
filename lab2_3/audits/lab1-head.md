# Code Smell Audit (lab1-head)

| Smell | Location | Confidence | Reason |
|---|---|---|---|
| **Large Class** | `src/Game.cpp:12` | C4 | The `Game` class handles too many responsibilities: rendering, input, game logic, and high scores. |
| **Long Method** | `src/Game.cpp:17` | C4 | `Game::GenerateObstacles()` is extremely long, generating 5 distinct patterns inline. |
| **Magic Numbers** | `src/Game.cpp:24` | C3 | Hardcoded constants for safe zones and obstacle patterns rather than constants. |
| **Feature Envy** | `src/Food.cpp:32` | C2 | The `Food` class accesses `Snake` internals to avoid spawning on it. |
| **Data Clumps** | `src/Game.h:17` | C4 | `snake` and `score` are duplicated as `snake2` and `score2`. The number of snakes is encoded in variable declarations. |
| **Shotgun Surgery** | `src/Game.cpp:45` | C4 | Adding a second snake required modifying collision checks, drawing, input, and score handling in multiple places manually. |
