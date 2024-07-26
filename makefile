# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra

# MSVC compiler and flags
MSVC = cl
MSVCFLAGS = /std:c++20 /EHsc /W4

# Source file
SRC = src/YTCompression.cpp

# Object file and directories
OBJ_DIR = ./obj
OBJ_FILE = $(OBJ_DIR)/YTCompression.obj

# Output executables
TARGET_WIN = ./bin/ytcomp.exe
TARGET_MSVC = ./bin/ytcomp_msvc.exe
TARGET_LINUX = ./bin/ytcomp

# Rule to build the Windows executable with g++
windows: $(TARGET_WIN)

# Rule to link the object file into the Windows executable with g++
$(TARGET_WIN): $(OBJ_FILE) | ./bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILE)

# Rule to build the Windows executable with MSVC
msvc: $(TARGET_MSVC)

# Rule to compile and link the object file into the Windows executable with MSVC
$(TARGET_MSVC): $(OBJ_FILE) | ./bin
	$(MSVC) $(MSVCFLAGS) /Fe$@ $(OBJ_FILE)

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

# Clean up generated files
clean:
	@rm -f $(TARGET_WIN) $(TARGET_MSVC) $(TARGET_LINUX) $(OBJ_FILE) ./bin/*-comp.* ./bin/*.log ./bin/*.txt

# Phony targets
.PHONY: windows msvc linux clean
