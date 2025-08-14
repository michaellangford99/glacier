
# Pre-compiler and Compiler flags
CXX_FLAGS := -ggdb#-Wall -Wextra -std=c++20 
PRE_FLAGS := -std=c++2a -MMD -MP

# Project directory structure
BIN := bin
SRC := src
LIB := lib
INC := include
CONTENT := content
BUILD := build

# Library search directories and flags
EXT_LIB :=
LDFLAGS := -pthread -ldl -lglfw -lfreetype
LDPATHS := $(addprefix -L,$(LIB) $(EXT_LIB))

# Include directories
INC_DIRS := $(INC) $(SRC) $(wildcard $(SRC)/*/) $(INC)/glm /usr/include/freetype2
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Construct build output and dependency filenames.
IMGUI_SRC_DIR := $(SRC)/imgui
IMGUI_SRCS := $(wildcard $(IMGUI_SRC_DIR)/*.cpp) \
			  $(IMGUI_SRC_DIR)/backends/imgui_impl_opengl3.cpp \
			  $(IMGUI_SRC_DIR)/backends/imgui_impl_glfw.cpp \
			  $(IMGUI_SRC_DIR)/misc/freetype/imgui_freetype.cpp	  \
			  $(IMGUI_SRC_DIR)/misc/cpp/imgui_stdlib.cpp	  

GLACIER_SRC_DIR := $(SRC)/glacier
GLACIER_SRCS := $(wildcard $(GLACIER_SRC_DIR)/*.cpp $(GLACIER_SRC_DIR)/*.c)

GLACIER_LIB_SRCS := $(GLACIER_SRCS) $(IMGUI_SRCS)
GLACIER_LIB_OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(GLACIER_LIB_SRCS))))
GLACIER_LIB_TARGET := $(BIN)/glacier_lib.a

DEMO_0_SRC_DIR := $(SRC)/target/demo_0
DEMO_0_SRCS := $(wildcard $(DEMO_0_SRC_DIR)/*.cpp $(DEMO_0_SRC_DIR)/*.c)
DEMO_0_OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(DEMO_0_SRCS))))
DEMO_0_TARGET := $(subst $(SRC)/,$(BIN)/, $(addsuffix .gl,$(DEMO_0_SRC_DIR)))

ALL_OBJS := $(GLACIER_LIB_OBJS) $(DEMO_0_OBJS)
DEPS := $(ALL_OBJS:.o=.d)

# Generate target names for content files
GLSL := $(subst $(SRC)/,$(BIN)/,$(shell find $(SRC) -name '*.glsl'))
FONT := $(subst $(SRC)/,$(BIN)/,$(shell find $(SRC) -name '*.ttf'))
CONTENT_TARGETS := $(subst $(CONTENT)/,$(BIN)/,$(wildcard content/*))

# Build task
build: all

# Main task
all: demo_0#here add the demo targets, and *they* will depend on glacier_lib
#	@echo $(SRCS)
#	@echo $<

demo_0: $(DEMO_0_TARGET)

glacier_lib: $(GLACIER_LIB_TARGET) $(GLSL) $(FONT) $(CONTENT_TARGETS)

$(DEMO_0_TARGET): $(DEMO_0_OBJS) glacier_lib 
	@echo "🚧 Building..."
	@mkdir -p $(dir $@)
	$(CXX) $(DEMO_0_OBJS) $(GLACIER_LIB_TARGET) -o $@ $(LDPATHS) $(LDFLAGS)

# Task producing target from built files
$(GLACIER_LIB_TARGET): $(GLACIER_LIB_OBJS)
	@echo "🚧 Building..."
#	@echo $(INC_FLAGS)
#	@echo $(SRCS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^
#	$(CXX) $(OBJS) -o $@ $(LDPATHS) $(LDFLAGS)

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
