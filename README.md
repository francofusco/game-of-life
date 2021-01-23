# Conway's Game of Life using piksel

A [piksel](https://bernhardfritz.github.io/piksel/#/)-based implementation of
the popular [*Game of Life*](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
created by [J.H.Conway](https://en.wikipedia.org/wiki/John_Horton_Conway).

This implementation uses a simple toroidal grid, meaning that the bottom and
top edges are considered adjacent, and the same goes for the left and right
edges.


## Requirements

- A C++ compiler supporting the C++11 standard
- CMake (version 3.12 or greater)


## Build

After cloning this repository, add piksel by typing:
```
git clone --recursive https://github.com/bernhardfritz/piksel.git
```
You can then compile the main executable using CMake:
```
mkdir build
cd build
cmake ..
make
```

If you want to modify the existing code, you might be interested in using the
`DEBUG(msg)` macro (defined in `src/debug.hpp`), which allows to print the
message `msg` on the terminal. You can used it in a `cout`-like style, *e.g.*,
```
int val = 0;
std::string s("hello")
DEBUG("Initialized 'val' to " << val << " and 's' to " << s);
```
To see the messages, you need to enable the "DEBUG" option as in
`cmake .. -DDEBUG=ON` or using CMake's GUI.


## "Play" the game

You can run the app using any of the following commands:
```
./game_of_life
./game_of_life size
./game_of_life width height
```
The second one creates a squared grid whose side is of given size, while the
last form allows to specify the grid dimensions individually.

Once the app is running, you should see a
[*Glider*](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Examples_of_patterns)
in the middle of the grid. You can click anywhere on the grid to create or
destroy a cell. Use the space bar to start/pause the game, and the "+" and "-"
keys on the numpad to increase/decrease the speed of the game.

Enjoy!
