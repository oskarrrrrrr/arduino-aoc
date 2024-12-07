# arduino build
BUILD_DIR = aoc
BOARD = arduino:renesas_uno:minima
PORT = /dev/cu.usbmodem2101

# local build
BIN_DIR = bin
CC = gcc
CFLAGS = -Wall -g

.PHONY: all
all:
	$(MAKE) aduino
	$(MAKE) local

.PHONY: arduino-all
arduino-all:
	$(MAKE) compile
	$(MAKE) upload
	$(MAKE) monitor

.PHONY: arduino
arduino:
	@mkdir -p $(BUILD_DIR)
	@cp aoc.h $(BUILD_DIR)
	@cp aoc.ino $(BUILD_DIR)
	cd $(BUILD_DIR); arduino-cli compile -b $(BOARD) aoc.ino

.PHONY: upload
upload:
	cd $(BUILD_DIR); arduino-cli upload aoc.ino --port $(PORT) -b $(BOARD)

.PHONY: upload
monitor:
	cd $(BUILD_DIR); arduino-cli monitor --quiet --port $(PORT)

.PHONY: local
local:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/aoc local.c

.PHONY: run
run:
	@cat input.txt | ./bin/aoc
