#define AOC_2024_04
#include "aoc.h"

const unsigned long INACTIVITY_TRESHOLD = 100;
unsigned long last_activity = millis();
int solved = 0;

void setup() {
    Serial.begin(9600);
    reset();
}

void loop() {
    unsigned long time = millis();
    int bytes = Serial.available();
    if (bytes > 0) {
        if (time - last_activity >= INACTIVITY_TRESHOLD) {
            reset();
            solved = 0;
        }
        last_activity = time;
        while (bytes--) {
            on_byte(Serial.read());
        }
    } else {
        if (time - last_activity >= INACTIVITY_TRESHOLD && solved == 0) {
            int part1, part2;
            solve(&part1, &part2);
            Serial.print("Part 1: ");
            Serial.println(part1);
            Serial.print("Part 2: ");
            Serial.println(part2);
            solved = 1;
        }
    }
}
