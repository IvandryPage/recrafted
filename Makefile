# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++20 -Iinclude

# Windows cross-compiler (MinGW)
CXX_WIN = x86_64-w64-mingw32-g++
CXXFLAGS_WIN = -static -std=c++20 -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Find all .cpp files automatically
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Output file names
TARGET_LINUX = $(BUILD_DIR)/recrafted
TARGET_WINDOWS = $(BUILD_DIR)/recrafted.exe

# Ensure build directory exists
$(shell mkdir -p $(OBJ_DIR))

# Build both Linux and Windows versions
all: $(TARGET_LINUX) $(TARGET_WINDOWS)

# Build Linux executable
$(TARGET_LINUX): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_LINUX) $(OBJS)

# Build Windows executable
$(TARGET_WINDOWS): $(OBJS)
	$(CXX_WIN) $(CXXFLAGS_WIN) -o $(TARGET_WINDOWS) $(SRCS)

# Compile .cpp to .o, ensuring obj directory exists
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure obj directory exists before compiling
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean compiled files
clean:
	rm -rf $(BUILD_DIR)