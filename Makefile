# Compiler
CXX := g++
CXXFLAGS := -g -std=c++20 -Iinclude

# Directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Sources
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    TARGET := $(BUILD_DIR)/recrafted
endif

ifeq ($(UNAME_S),Darwin)
    TARGET := $(BUILD_DIR)/recrafted
endif

# Windows via MinGW/MSYS
ifeq ($(OS),Windows_NT)
    TARGET := $(BUILD_DIR)/recrafted.exe
endif

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run