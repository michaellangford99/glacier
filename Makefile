
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
TARGET_PROJECT := glacier
TARGET := $(BIN)/$(TARGET_PROJECT)/$(TARGET_PROJECT)
BUILD := build

# Library search directories and flags
EXT_LIB :=
LDFLAGS := -pthread -ldl -lglfw -lfreetype
LDPATHS := $(addprefix -L,$(LIB) $(EXT_LIB))

# Include directories
INC_DIRS := $(INC) $(wildcard $(SRC)/*/) $(INC)/glm /usr/include/freetype2 $(SRC)/.
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Construct build output and dependency filenames.
IMGUI_SRC_DIR := $(SRC)/imgui
IMGUI_SRCS := $(wildcard $(IMGUI_SRC_DIR)/*.cpp) \
			  $(IMGUI_SRC_DIR)/backends/imgui_impl_opengl3.cpp \
			  $(IMGUI_SRC_DIR)/backends/imgui_impl_glfw.cpp \
			  $(IMGUI_SRC_DIR)/misc/freetype/imgui_freetype.cpp	  

#SRCS := $(shell find src -name '*.c') $(shell find src -name '*.cpp')
SRCS := $(wildcard $(SRC)/$(TARGET_PROJECT)/*.cpp $(SRC)/$(TARGET_PROJECT)/*.c) $(IMGUI_SRCS)
OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))
DEPS := $(OBJS:.o=.d)

# Generate target names for content files
GLSL := $(subst $(SRC)/,$(BIN)/,$(shell find $(SRC) -name '*.glsl'))
FONT := $(subst $(SRC)/,$(BIN)/,$(shell find $(SRC) -name '*.ttf'))
CONTENT_TARGETS := $(subst $(CONTENT)/,$(BIN)/,$(wildcard content/*))

TEST_TARGETS := a b c d e

# Build task
build: all

# Main task
all: $(TARGET) $(GLSL) $(FONT) $(CONTENT_TARGETS) $(TEST_TARGETS)
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

$(TEST_TARGETS):
	@echo $@

#OK. so each of TEST_TARGET depends on GLACIER
#GLACIER depends on all the content file copies I guess (not sure, maybe split that into each one? sounds like a waste)
#and then make all makes all the tests, glacier, copies all content
#and you can specify make glacier
#make volume_test etc. and so on

#OK but what about where the executable should go?
#going to put it in subdirectory but execute from root bin. seems a little janky.....

#also copy all headers to inc/ directory on make so that we can use <> to access headers non-locally?
#and not depend on Make to fix paths for us

# Clean task
.PHONY: clean
clean:
	rm -rf $(BUILD)/*
	rm -rf $(BIN)/*

# Include all dependencies
-include $(DEPS)
