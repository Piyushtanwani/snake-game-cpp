# Ubiquitous Language

## Entities

| Term | Definition | Aliases to avoid | In code |
|---|---|---|---|
| **Board** | The bounded grid where the game takes place | Screen, Grid | *(absent)* |
| **Snake** | A player-controlled entity composed of segments | Player, Worm | `Snake` — `src/Snake.h:12` |
| **Segment** | A single coordinate piece of a Snake's body | Part, Block | `Segment` — `src/Snake.h:8` |
| **Food** | An item the snake consumes to gain points and grow | Fruit, Pellet | `Food` — `src/Food.h:8` |
| **Obstacle** | A static coordinate block that causes a collision if hit | Wall, Barrier | `obstacles` — `src/Game.h:23` |
| **Tick** | A single step of game logic and movement time | Frame, Turn | *(absent)* |
| **Score** | The accumulated points of a player | Points | `score` — `src/Game.h:24` |
| **Collision** | When a snake hits an obstacle, a boundary, or another snake | Crash, Impact | `CheckSelfCollision` — `src/Snake.h:25` |

## Enums and States

| Term | Definition | Aliases to avoid | In code |
|---|---|---|---|
| **Direction** | The current heading of a Snake | Heading, Velocity | `Direction` — `src/Snake.h:6` |
| **FoodType** | The kind of food determining point value | Category | `FoodType` — `src/Food.h:10` |

## Flagged ambiguities

- "score" and "points" are used interchangeably (e.g., `score` in Game, but `GetPoints()` in Food). A **Score** is the total, while **points** are what a Food grants.
- "player" versus "snake": The user plays the game as a Player, but the code strictly refers to `snake` and `snake2`.

## Code drift

| Canonical term | Called in code | Location | Note |
|---|---|---|---|
| **Board** | *(absent)* | — | Modeled implicitly via `WIDTH` and `HEIGHT` boundaries, but has no explicit representation. |
| **Tick** | *(absent)* | — | Governs the game loop pace but is not named in the domain models. |
