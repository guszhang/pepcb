CC = g++
CFLAGS = -Isrc/connector -Isrc/core -g --std=c++17
SRC = src/connector/connector_kicad.cpp src/test/test.cpp 

.PHONY: test exec

test:
	@$(CC) $(SRC) -o build/test $(CFLAGS)

exec:
	@./build/test