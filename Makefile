CC = g++
CFLAGS = -Isrc/connector -Isrc/core -g
SRC = src/connector/connector_kicad.cpp src/test/test.cpp 

.PHONY: test exec

test:
	@$(CC) $(SRC) -o build/test $(CFLAGS)

exec:
	@./build/test