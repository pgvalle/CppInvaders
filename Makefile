# Directories
SRC = src
OUT = out
PICO_SRC = pico/src
PICO_OUT = out/pico
EXEC = cppinv.out

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -g -std=c++23
PKG_CONFIG = pkg-config
PKG_FLAGS = $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_image SDL2_ttf SDL2_gfx SDL2_mixer)
LDFLAGS = $(shell $(PKG_CONFIG) --libs sdl2 SDL2_image SDL2_ttf SDL2_gfx SDL2_mixer)

# Find all source files
SRC_FILES = $(shell find $(SRC) -type f -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC)/%.cpp, $(OUT)/%.o, $(SRC_FILES))
PICO_SRC_FILES = $(shell find $(PICO_SRC) -type f -name '*.c')
PICO_OBJ_FILES = $(patsubst $(PICO_SRC)/%.c, $(PICO_OUT)/%.o, $(PICO_SRC_FILES))

# Target to build the executable
$(EXEC): $(OBJ_FILES) $(PICO_OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(PICO_OBJ_FILES) -o $@ $(LDFLAGS)

# Rule to compile C++ source files
$(OUT)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(PICO_SRC) $(PKG_FLAGS) -c $< -o $@

# Rule to compile C source files (pico-sdl)
$(PICO_OUT)/%.o: $(PICO_SRC)/%.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(PKG_FLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -rf $(OUT) $(EXEC)

# Rebuild
.PHONY: all
all: clean $(EXEC)
