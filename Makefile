# Directories
SRC_DIR = src
PICO_DIR = deps/pico-sdl/src
OUT_DIR = out
EXECUTABLE = cppi.out

# Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -Wall -Wextra -g -std=c++17 -I$(PICO_DIR)
CFLAGS = -Wall -Wextra -g -std=c11
PKG_CONFIG = pkg-config
PKG_FLAGS = $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_image SDL2_ttf SDL2_gfx SDL2_mixer)
LDFLAGS = $(shell $(PKG_CONFIG) --libs sdl2 SDL2_image SDL2_ttf SDL2_gfx SDL2_mixer)

# Find all source files
CPP_SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.cpp')
PICO_SRC_FILES = $(shell find $(PICO_DIR) -type f -name '*.c')
CPP_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OUT_DIR)/%.o, $(CPP_SRC_FILES))
PICO_OBJ_FILES = $(patsubst $(PICO_DIR)/%.c, $(OUT_DIR)/deps/%.o, $(PICO_SRC_FILES))
OBJ_FILES = $(CPP_OBJ_FILES) $(PICO_OBJ_FILES)

# Target to build the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Rule to compile C++ source files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(PKG_FLAGS) -c $< -o $@

# Rule to compile C source files (pico-sdl)
$(OUT_DIR)/deps/%.o: $(PICO_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(PKG_FLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -rf $(OUT_DIR) $(EXECUTABLE)

# Rebuild
.PHONY: all
all: clean $(EXECUTABLE)