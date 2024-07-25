# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Source file
SRC = src/YTCompression.cpp

# Object file and directories
OBJ_DIR = ./obj
OBJ_FILE = $(OBJ_DIR)/Object.o

# Output executables
TARGET_WIN = ./bin/yt_compression.exe
TARGET_LINUX = ./bin/yt_compression

# Rule to build the Windows executable
windows: $(TARGET_WIN)

# Rule to link the object file into the Windows executable
$(TARGET_WIN): $(OBJ_FILE) | ./bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILE)

# Rule to build the Linux executable
linux: $(TARGET_LINUX)

# Rule to link the object file into the Linux executable
$(TARGET_LINUX): $(OBJ_FILE) | ./bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILE)

# Rule to compile the source file into the object file
$(OBJ_FILE): $(SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ_FILE)

# Ensure the bin directory exists
./bin:
	@mkdir -p ./bin

# Ensure the obj directory exists
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

COMPVID = ./bin/*-comp.*
# Rule to clean up generated files
clean:
	rm -f $(TARGET_WIN) $(TARGET_LINUX) $(OBJ_FILE) $(COMPVID) ./bin/*.log ./bin/*.txt

# Phony targets
.PHONY: windows linux clean