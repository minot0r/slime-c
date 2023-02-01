# SLIME

This repository contains the source code for the SLIME game. This game is heavily inspired by: One Slime, a gamed designed and created by Daniel Wedge in 2001.
This version is rewritten in C with no intentions to steal the original idea, but to create a new version of the game with a new engine. It is designed to be played on an arcade but can still be played on a computer.

Note: The game is optimized for 5:4 aspect ratio. So on any other aspect ratio monitors it is recommended to play in windowed mode.

---

## How to play

The game is designed to be played on an arcade machine. The controls are as follows:

- Q: Player 1 Left
- Z: Player 1 Jump
- D: Player 1 Right
- Left Arrow: Player 2 Left, Select in menu
- Up Arrow: Player 2 Jump, Go next in menu
- Right Arrow: Player 2 Right

---

## How to build

This game is runned on the SDL2 library.
You can wether build it with the makefile or download the precompiled version for your OS.

### Build with makefile

#### Libs
Once you have all the submodules, run :

```bash
make libs
```

If you want any of the particular libraries to be built, you can run :

```bash
make sdl
or
make sdl-image
or
make sdl-ttf
```

#### Game

Once you have all the libraries, you can build the game with :

```bash
make main
```

The executable will be in the bin folder.