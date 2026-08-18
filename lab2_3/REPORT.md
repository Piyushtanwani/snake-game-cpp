# Lab 2_3 — Group A


---

## 1. Tool and install route — [3]

| | |
|---|---|
| Agent used for run 2 | Antigravity IDE (Gemini 3.1 Pro) |
| `ubiquitous-language` install route | Copied to workspace `.agents/skills` |
| `refactoring/` pack install route | Copied to workspace `.agents/skills` |

All skills installed successfully into the `.agents/skills` directory, utilizing Antigravity IDE's native skills loading mechanism.

---

## 2. What I changed in the glossary — [4]

- Removed "Screen" and "Grid" as aliases because the domain experts refer to the bounding area strictly as the **Board**.
- Clarified the ambiguity between "Score" and "Points": **Score** is the total for a player, while **points** are granted by Food.
- Flagged the code drift regarding "Player": the game is played by a Player, but the code only knows `snake` and `snake2`.
- Noted that **Tick** and **Board** are absent from the codebase models despite being implicit in the domain loop and boundaries.

---

## 3. Smell delta — [6]

Reports: `lab2_3/audits/main.md` (the code as you received it) and `lab2_3/audits/lab1-head.md`
(after your Lab-1 PR).

| | count | representative site (`file:line`) |
|---|---|---|
| Smells my Lab-1 PR **introduced** | 1 | `src/Game.cpp:45` |
| Smells my Lab-1 PR **left untouched** | 5 | `src/Game.cpp:12` |
| Smells my Lab-1 PR **removed** | 0 | — |


---

## 4. Rejected candidates — [6]

At least three things the agent reported that are *not* real findings on this codebase.

| smell reported | `file:line` | why it does not hold |
|---|---|---|
| Feature Envy | `src/Food.cpp:32` | `Food` naturally needs to know where the snake is to avoid spawning inside it; this is a logical spatial constraint, not a design flaw. |
| Magic Numbers | `src/Game.cpp:24` | While they are literal numbers, they are part of purely visual layout generation and extracting them to named constants wouldn't improve readability. |
| Large Class | `src/Game.cpp:12` | In a simple console game, breaking out rendering and input into separate abstract classes might be overengineering and violate KISS. |

---

## 5. Commit map — [7]

| # | sha | subject | what it is |
|---|---|---|---|
| 1 | 7572f0e | Commit 1: Part B UBIQUITOUS_LANGUAGE.md | glossary |
| 2 | cfa8c37 | Commit 2: Part C smell reports | smell report |
| 3 | 705407e | refactor: represent snake and score as collections | the refactor alone |
| 4 | bda68cf | feat: add second player | the feature alone |

---

## 6. Two-run measurement — [4]

| | Run 1 (Lab 1) | Run 2 (commit 4) |
|---|---|---|
| Smells introduced | 1 | 0 |
| Lines changed, `git diff --shortstat -w` | 7 files changed, 271 insertions(+), 45 deletions(-) | 4 files changed, 97 insertions(+), 61 deletions(-) |
| Lines changed, **raw** (no `-w`) | 19 files changed, 1756 insertions(+), 1530 deletions(-) | 4 files changed, 117 insertions(+), 81 deletions(-) |
| Functions reached | ~11 | ~11 |
| Prompts to working code | 1 | 1 |
| Wall-clock time | Not recorded during Lab 1, and no timestamps were preserved in the logs to reconstruct it | ~48 minutes |

Commit 3 (the refactor) on its own: 4 files changed, 81 insertions(+), 45 deletions(-) lines `-w`, 4 files changed, 125 insertions(+), 89 deletions(-) raw.


---

## 7. Analysis Q1–Q2 — [5]

**Q1. Which smell did commit 3 actually fix?** Name it from your section 3 report. What was
expensive before, what does it cost now.

Commit 3 fixed the "Data Clumps" smell in `src/Game.h:17` (`snake`, `score`). Before the refactor, adding a new snake required expensive manual duplication of discrete variables and duplicating movement/collision logic across the codebase. Now, we use iterative collections (`std::vector<Snake>`), centralizing the state representation. This significantly reduced the need for duplicated logic and Shotgun Surgery, allowing new players to be processed natively by the game loop.

**Q2. Compare commit 4 to your Lab-1 diff.** Same feature, same codebase. What changed in
the cost and what did not? If it got worse, say so and explain — that marks the same.

Both runs touched approximately the same number of functions (~11) because `Game.cpp` remained a massive monolith with God-methods. However, the cost of adding the feature dropped significantly in Run 2. In Lab 1, we manually duplicated logic and introduced Shotgun Surgery, costing 271 insertions (-w). In Run 2, leveraging the collections introduced in Commit 3, we implemented the feature safely with only 109 insertions (-w), keeping the code clean and avoiding any new smells.

---

## 8. Analysis Q3–Q4 — [5]

**Q3. Go back through your Lab-1 `LLM-LOG.md`. Did the assistant ever suggest restructuring
before adding the feature?** Quote it if it did. If it did not, what would have had to be
different in your prompt?

The assistant did not suggest restructuring before adding the feature. This was primarily because the Lab 1 prompt explicitly enforced anti-refactoring constraints: "Do NOT refactor the code", "Do NOT clean up existing code", and "DO NOT FIX UNRELATED ISSUES". To receive restructuring suggestions, the prompt would have needed to explicitly authorize or encourage identifying and resolving design flaws before feature implementation.

**Q4. How do you know commit 3 did not change behaviour?** Answer honestly. Most of you will
find that you do not know. Say that plainly if it is true, and describe what you would have
needed in order to actually know.

Honestly, we cannot know with absolute certainty that commit 3 did not change behavior. We verified it by manually playing the game and checking collisions and scoring, but manual testing is subjective and error-prone. To definitively know that behavior was preserved, we would have needed a robust suite of automated tests (e.g., unit tests for game logic or boundary tests for collisions) to programmatically assert identical states before and after the refactor.

---


## If you did not finish

Not applicable. I finished all sections of the lab.
