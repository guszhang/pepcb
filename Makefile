CC = g++
CFLAGS = -Isrc/connector -Isrc/core -g --std=c++17
SRC = src/connector/connector_kicad.cpp src/test/test.cpp
SRCUI = src/ui/ui.cpp

.PHONY: test exec ui

test:
	@$(CC) $(SRC) -o build/test $(CFLAGS)

exec:
	@./build/test

ui:
	@$(CC) $(SRCUI) -o build/ui -lglfw -lGL