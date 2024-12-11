# Advent of Code on Arduino

[Advent of Code](https://adventofcode.com) on Arduino Uno R4 Minima.

**Arduino Specs**
- Clock Speed: 48 MHz
- Memory: 256 kB Flash, 32 kB RAM

With 32kB of RAM, at least some of the problems should be possible to solve on
an Arduino. I chose not to upload the input as a part of the program because I
think it's more interesting this way. As a result, every solution must handle
reading the input character by character.

## Running


### On Arduino

In `aoc.ino` change the first line to choose the solution to compile.
You need an aforementioned Arduino board and `arduino-cli`. Adjust the `PORT`
variable in the Makefile and run `make arduino-all`. This will compile the
solution, upload the program to the board and run `monitor` which let's you
communicate with the board. Then paste the input to the terminal.

### Locally

In `local.c` change the first line to choose the solution to compile.
Compile with `make local` and run, for instance, like this: `cat input.txt |
./bin/aoc` (shortcut: `make run`). Note that locally the memory is not limited
to 32 kB.

## Memory Usage

I'm not trying to optimize every byte so the numbers below definitely can be
improved. For instance, by making various arrays in the code just large enough
to hold the input.

| Problem | Flash \[bytes\] | RAM \[bytes\] | Notes |
| --- | --- | --- | --- |
| 2024-01 | 40108 | 11960 | |
| 2024-02 | 39532 | 4044 | |
| 2024-03 | 39872 | 4024 | |
| 2024-04 | 39908 | 19964 | |
| 2024-05 | - | - | doesn't work on arduino |
| 2024-06 | - | - | only part 1 done |
