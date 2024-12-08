#define AOC_2024_04
#include "aoc.h"

#include <unistd.h>
#include <stdio.h>

int main() {
    reset();
    char byte;
    while(read(STDIN_FILENO, &byte, 1) > 0) {
        on_byte(byte);
    }
    int part1, part2;
    solve(&part1, &part2);
    printf("Part 1: %d\n", part1);
    printf("Part 2: %d\n", part2);
}
