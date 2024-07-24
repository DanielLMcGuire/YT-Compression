# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Source file
SRC = YTCompression.cpp

# Object file and directories
OBJ_DIR = ./obj
OBJ_FILE = $(OBJ_DIR)/Object.o

# Output executables
TARGET_WIN = ./bin/yt_compression.exe
TARGET_LINUX = ./bin/yt_compression

# Default rule to build both Windows and Linux targets
all: win linux

# Rule to build for Windows
win: $(TARGET_WIN)

# Rule to link the object file into the Windows executable
$(TARGET_WIN): $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILE)

# Rule to build for Linux
linux: $(TARGET_LINUX)

# Rule to link the object file into the Linux executable
$(TARGET_LINUX): $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILE)

# Rule to compile the source file into the object file
$(OBJ_FILE): $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ_FILE)



COMPVID = ./bin/*-comp.*
# Rule to clean up generated files
clean:
	rm -f $(TARGET_WIN) $(TARGET_LINUX) $(OBJ_FILE) $(COMPVID) ./bin/*.log ./bin/*.txt

# Phony targets
.PHONY: all win linux clean
