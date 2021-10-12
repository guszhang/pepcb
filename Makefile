CC = g++
CFLAGS = -Isrc/connector -Isrc/core -g --std=c++17
SRC = src/connector/connector_kicad.cpp src/test/test.cpp
SRCUI = src/ui/ui.cpp src/core/core.cpp

.PHONY: test exec ui

test:
	@$(CC) $(SRC) -o build/test $(CFLAGS)

exec:
	@./build/test

ui:
	@$(CC) $(SRCUI) -Isrc/core -g --std=c++17 -o build/ui -lglfw -lGL -lSOIL