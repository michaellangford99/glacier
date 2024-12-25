
# Pre-compiler and Compiler flags
CXX_FLAGS := -ggdb#-Wall -Wextra -std=c++20 
PRE_FLAGS := -std=c++2a -MMD -MP

# Project directory structure
BIN := bin
SRC := src
LIB := lib
INC := include
CONTENT := content

# Build directories and output
TARGET := $(BIN)/glacier
BUILD := build

# Library search directories and flags
EXT_LIB :=
LDFLAGS := -pthread -ldl -lglfw -lfreetype
LDPATHS := $(addprefix -L,$(LIB) $(EXT_LIB))

# Include directories
INC_DIRS := $(INC) $(wildcard $(SRC)/*/) $(INC)/glm /usr/include/freetype2
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Construct build output and dependency filenames.
IMGUI_SRC_DIR := $(SRC)/imgui
IMGUI_SRCS := $(wildcard $(IMGUI_SRC_DIR)/*.cpp) \
			  $(IMGUI_SRC_DIR)/backends/imgui_impl_opengl3.cpp \
			  $(IMGUI_SRC_DIR)/backends/imgui_impl_glfw.cpp \
			  $(IMGUI_SRC_DIR)/misc/freetype/imgui_freetype.cpp	  

#SRCS := $(shell find src -name '*.c') $(shell find src -name '*.cpp')
SRCS := $(wildcard $(SRC)/*.cpp $(SRC)/*.c) $(IMGUI_SRCS)
OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))
DEPS := $(OBJS:.o=.d)

# Generate target names for content files
GLSL := $(subst $(SRC)/,$(BIN)/,$(shell find $(SRC) -name '*.glsl'))
FONT := $(subst $(SRC)/,$(BIN)/,$(shell find $(SRC) -name '*.ttf'))
CONTENT_TARGETS := $(subst $(CONTENT)/,$(BIN)/,$(wildcard content/*))

# Build task
build: all

# Main task
all: $(TARGET) $(GLSL) $(FONT) $(CONTENT_TARGETS)
#	@echo $(SRCS)
#	@echo $<

# Task producing target from built files
$(TARGET): $(OBJS)
	@echo "🚧 Building..."
#	@echo $(INC_FLAGS)
#	@echo $(SRCS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDPATHS) $(LDFLAGS)

# Compile all cpp files
$(BUILD)/%.o: $(SRC)/%.c*
#	@echo $(INC_FLAGS)
#	@echo $(SRCS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(PRE_FLAGS) $(INC_FLAGS) -c -o $@ $< $(LDPATHS) $(LDFLAGS)

# Copy all glsl files to bin/
$(BIN)/%.glsl: $(SRC)/%.glsl
	@mkdir -p $(dir $@)
	cp $< $@

# Copy all ttf font files to bin/
$(BIN)/%.ttf: $(SRC)/%.ttf
	@mkdir -p $(dir $@)
	cp $< $@

# Copy everything in content/ to bin/
$(BIN)/%: $(CONTENT)/%
	@mkdir -p $(dir $@)
	cp $< $@

# Clean task
.PHONY: clean
clean:
	rm -rf $(BUILD)/*
	rm -rf $(BIN)/*

# Include all dependencies
-include $(DEPS)
