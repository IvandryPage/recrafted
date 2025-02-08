# Compiler and FLags
CXX = g++
CXXFLAGS = -g -std=c++20 -Iinclude

CXX_WIN = x86_64-w64-mingw32-g++
CXXFLAGS_WIN = -static -std=c++20 -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Output
TARGET_LINUX = $(BUILD_DIR)/recrafted
TARGET_WINDOWS = $(BUILD_DIR)/recrafted.exe

# Commands
$(shell mkdir -p $(OBJ_DIR))

all: $(TARGET_LINUX) $(TARGET_WINDOWS)

$(TARGET_LINUX): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_LINUX) $(OBJS)

$(TARGET_WINDOWS): $(OBJS)
	$(CXX_WIN) $(CXXFLAGS_WIN) -o $(TARGET_WINDOWS) $(SRCS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)