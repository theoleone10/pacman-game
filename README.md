# Pacman Game

A simple terminal-based Pac-Man style game written in C. The project was created for the APSC 142 course and demonstrates basic use of dynamic memory, file I/O, and automated testing.

## Project structure

- `apsc142project.c` – entry point that initializes the map, processes input and drives gameplay.
- `map.c` / `map.h` – loading and rendering of map files and wall detection.
- `actor.c` / `actor.h` – player and ghost movement and line-of-sight logic.
- `game.c` / `game.h` – win/lose conditions.
- `colours.c` / `colours.h` – helper utilities for coloured terminal output.
- `tests.cpp` – unit tests using [doctest](https://github.com/doctest/doctest).

## Building

This project uses CMake. From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

## Running the game

After building, the game executable is located in the `build` directory:

```bash
./build/project
```

Use the `w`, `a`, `s`, and `d` keys to move the player, eat all dots, and avoid the ghosts.

## Running tests

Automated tests are built alongside the game:

```bash
./build/tests
```

## Maps

Two sample maps (`map.txt`, `map2.txt`) are included. Maps are plain-text files where:

- `P` is the player start position
- `G` is a ghost
- `W` is a wall
- `.` is a dot the player must eat
- spaces are empty tiles

You can modify these files or create your own maps to change the layout of the game.

## License

This project is distributed for educational purposes and retains any copyrights noted in the source files.

