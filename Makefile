.PHONY: all
all:
	$(MAKE) compile
	$(MAKE) upload
	$(MAKE) monitor

.PHONY: compile
compile:
	arduino-cli compile -b arduino:renesas_uno:minima aoc_2024_02.ino

.PHONY: upload
upload:
	arduino-cli upload aoc_2024_02.ino --port /dev/cu.usbmodem2101 -b arduino:renesas_uno:minima

.PHONY: upload
monitor:
	arduino-cli monitor --port /dev/cu.usbmodem2101 --quiet
