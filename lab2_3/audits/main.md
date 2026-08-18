# Code Smell Audit (main)

| Smell | Location | Confidence | Reason |
|---|---|---|---|
| **Large Class** | `src/Game.cpp:12` | C4 | The `Game` class handles too many responsibilities: rendering, input, game logic, and high scores. |
| **Long Method** | `src/Game.cpp:17` | C4 | `Game::GenerateObstacles()` is extremely long, generating 5 distinct patterns inline. |
| **Magic Numbers** | `src/Game.cpp:24` | C3 | Hardcoded constants for safe zones and obstacle patterns rather than constants. |
| **Feature Envy** | `src/Food.cpp:32` | C2 | The `Food` class accesses `Snake` internals to avoid spawning on it. |
| **Data Clumps** | `src/Game.h:17` | C3 | `snake` and `score` always change and are passed together. The number of snakes is tied to discrete variables. |
