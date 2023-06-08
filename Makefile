CC = g++
CFLAGS = -Isrc/connector -Isrc/core -g --std=c++17
SRC = src/connector/connector_kicad.cpp src/test/test.cpp
SRCMAIN = src/core/core.cpp src/connector/s_expr.cpp src/connector/connector_kicad_footprint.cpp src/ui/ui.cpp src/ui/common/shader.cpp src/ui/common/layers.cpp src/main.cpp
SRCUI = src/ui/ui.cpp src/core/core.cpp src/ui/common/shader.cpp src/ui/common/layers.cpp

.PHONY: test exec ui uimac main

main:
	@$(CC) $(SRCMAIN) -o build/test $(CFLAGS) -lglfw -lGL -lSOIL

main-mac:
	@$(CC) $(SRCMAIN) /usr/local/lib/libSOIL.a -Isrc/core -Isrc/ui/common -I/opt/homebrew/Cellar/glfw/3.3.8/include -I/usr/local/include/SOIL -g --std=c++17 -o build/test -L/opt/homebrew/Cellar/glfw/3.3.8/lib -framework Cocoa -framework OpenGL -framework IOKit -lglfw

test:
	@$(CC) $(SRC) -o build/test $(CFLAGS)

exec:
	@./build/test

ui:
	@$(CC) $(SRCUI) -Isrc/core -g --std=c++17 -o build/ui -lglfw -lGL -lSOIL

uimac:
	@$(CC) $(SRCUI) /usr/local/lib/libSOIL.a -Isrc/core -Isrc/ui/common -I/opt/homebrew/Cellar/glfw/3.3.8/include -I/usr/local/include/SOIL -g --std=c++17 -o build/ui -L/opt/homebrew/Cellar/glfw/3.3.8/lib -framework Cocoa -framework OpenGL -framework IOKit -lglfw