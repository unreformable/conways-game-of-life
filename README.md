# Conway's Game of Life

Simple Conway's Game of Life simulator using C++ and SFML.


## Rules

At each step in time, the following transitions occur:

- Any live cell with fewer than two live neighbours dies, as if by underpopulation.
- Any live cell with two or three live neighbours lives on to the next generation.
- Any live cell with more than three live neighbours dies, as if by overpopulation.
- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

## Control

Base:

- Space - start/stop simulation
- LMB - set cell
- RMB - remove cell
- R - reset grid
- Ctrl+C - save state of grid
- Ctrl+V - load saved state of grid
- Up/Down - increase/decrease duration of simulation step

Camera:

- W,A,S,D - move camera around
- Shift - increases speed of camera
- Ctrl - decreases speed of camera
- Scroll - zoom in/out

## Reference

If you would like to learn more I recommend [this](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) site.