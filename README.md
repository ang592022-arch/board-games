# Board Games

A small C++17 console project containing two playable board games, an
N-Queen solver, and several independently tested programming exercises.

Originally developed as a university programming exercise and later
reorganized and refactored into a standalone project.

## Included modules

| Module | Status | Behaviour |
| --- | --- | --- |
| Gomoku | Playable | Two-player turns, move validation, horizontal/vertical/diagonal five-in-a-row detection, draw detection, reset, and move replay |
| N-Queen Duel | Playable | Two players place non-attacking queens; attempting a threatened square loses immediately, while placing the required number of queens or leaving no legal square wins |
| N-Queen Solver | Available | Recursive backtracking for one solution or all solutions |
| MyVector, Menu, Stones | Exercise components | Standalone data-structure and ownership exercises covered by automated checks; the games do not depend on them |

Save and load are not implemented. Gomoku replay is available after a
completed game.

## Features

- A menu-driven console application for Gomoku, N-Queen Duel, and the
  N-Queen solver.
- A small board model with bounds checks, cell access, clearing, and ASCII
  rendering.
- Game rules separated from command-line input and output.
- Gomoku move history and deterministic replay.
- Dependency-free automated tests for rules, solver behaviour, input
  handling, and the retained exercise components.
- An optional interactive 9x9 board-editing example.

## Project layout

```text
board-games/
|-- app/                         # Application entry point
|-- examples/                    # Optional board-editing example
|-- include/board_games/
|   |-- algorithms/              # N-Queen solver interface
|   |-- cli/                     # Stream-based command-line interface
|   |-- core/                    # Board, input, and text helpers
|   |-- exercises/               # MyVector, Menu, and Stones exercises
|   `-- games/                   # Gomoku and N-Queen Duel
|-- src/                         # Implementations
|-- tests/                       # Automated test runner
|-- .gitignore
|-- Makefile
`-- README.md
```

## Requirements

- A C++ compiler accepting C++17 mode.
- GNU Make, or `mingw32-make` with MinGW on Windows.

The implementation uses a C++11/14-compatible subset of the language so that
it also builds with the available GCC 6.3 MinGW toolchain in C++17 mode.

## Build and test

From the `board-games/` directory:

```sh
make
make test
```

On Windows with MinGW:

```powershell
mingw32-make
mingw32-make test
```

The default target builds:

- `build/bin/board-games` (or `board-games.exe`)
- `build/bin/board-games-tests` (or `board-games-tests.exe`)

Other targets:

```sh
make app       # Build the main application
make tests     # Build the test executable
make run       # Run the main application
make demo      # Build the optional board editor
make clean     # Remove generated build output
```

Replace `make` with `mingw32-make` when using MinGW.

## Run and controls

Run the application:

```sh
./build/bin/board-games
```

On Windows:

```powershell
.\build\bin\board-games.exe
```

The CLI prompts for the row and numeric column separately. Enter `0` at a
row prompt to return to the main menu.

- Gomoku accepts odd board sizes from 5 to 25. Players alternate `X` and
  `O`; five consecutive stones in any supported direction wins.
- N-Queen Duel accepts even board sizes from 4 to 16. A safe placement changes
  the turn. A threatened placement is not added to the board and immediately
  loses for the player who attempted it. The player making the final required
  safe placement wins; if a safe placement leaves no legal square, that player
  also wins.
- N-Queen Solver accepts sizes from 1 to 12 and can print one solution or all
  solutions.

The optional board example accepts a row, a lettered column, and `black` or
`white`. Lettered board coordinates are case-insensitive and omit column
`I`.

## Technical design

`Board` owns a flat standard-library container and only manages rectangular
cell state and rendering. `GomokuGame` and `NQueenDuel` each own a
`Board` and their own turn, history, and outcome state; there is no shared
game-engine inheritance hierarchy. The CLI works with input/output streams, so
its interaction paths can be tested without launching a subprocess.

`NQueenSolver` is independent of the playable duel and retains the recursive
backtracking approach. `MyVector`, `Menu`, and `Stones` are isolated
exercise components rather than dependencies of the game model.

The program uses ASCII symbols to avoid platform-specific console APIs. It has
no third-party runtime or test dependencies.

## Current limitations

- The interface is console-only and supports human players only.
- Gomoku supports replay but not persistent save files.
- N-Queen Solver output grows quickly for larger boards.
- No networking or computer opponent is included.

## Possible extensions

- Add a versioned, independently designed Gomoku save format.
- Add computer opponents behind the existing rule APIs.
- Add more malformed-input and property-style rule tests.
- Build another front end on top of the stream-independent game modules.
