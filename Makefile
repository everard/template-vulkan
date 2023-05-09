# Name of the executable file.
TARGET_NAME=game

# Directories.
DIR_BUILD=build
DIR_SHADERS=shaders

# Compilers.
CPP = g++
GLSL_COMPILER = glslangValidator

# C++ compiler parameters.
CFLAGS =\
 -Wall -Wextra -Wno-missing-field-initializers -O2 -std=c++23 \
 $(shell pkg-config --cflags sdl2) \
 $(shell pkg-config --cflags vulkan)

CLIBS =\
 $(shell pkg-config --libs sdl2) \
 $(shell pkg-config --libs vulkan)

# C++ source files.
SRC = $(sort $(wildcard src/*.cc))

# Shader source files.
SRC_SHADERS = $(sort $(wildcard $(DIR_SHADERS)/*.comp))
SRC_SHADERS += $(sort $(wildcard $(DIR_SHADERS)/*.vert))
SRC_SHADERS += $(sort $(wildcard $(DIR_SHADERS)/*.frag))

# C++ object files.
DEPS_OBJ = $(patsubst src/%.cc,$(DIR_BUILD)/%.o,$(SRC))

# SPIR-V shader files.
DEPS_SHADERS = $(patsubst $(DIR_SHADERS)/%,$(DIR_SHADERS)/%.spv,$(SRC_SHADERS))

# Make rules.
program: $(DEPS_OBJ)
	$(CPP) $(CFLAGS) $^ $(CLIBS) -o $(DIR_BUILD)/$(TARGET_NAME)

shaders: $(DEPS_SHADERS)

clean:
	rm -f $(DIR_BUILD)/$(TARGET_NAME)
	rm -f $(DIR_BUILD)/*.o

$(DIR_BUILD)/%.o: src/%.cc
	$(CPP) $(CFLAGS) -c -o $@ $<

$(DIR_SHADERS)/%.spv: $(DIR_SHADERS)/%
	$(GLSL_COMPILER) -V $< -o $@
