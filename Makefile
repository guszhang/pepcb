CC = clang++
CFLAGS = -Isrc/connector -Isrc/core
SRC = src/connector/connector_kicad.cpp src/test/test.cpp 

.PHONY: build exec

build:
	@$(CC) $(SRC) -o build/test $(CFLAGS)

exec:
	@./build/test