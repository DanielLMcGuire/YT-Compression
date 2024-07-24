# Compiler
CC = gcc

# Include directory for the headers in the repository
INCLUDE_DIR = ./include

# Library files
LIBS = avcodec.lib avformat.lib avutil.lib swscale.lib

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -I$(INCLUDE_DIR)

# Linker flags
LDFLAGS = -L. $(LIBS)

# Source files
SRCS = YTCONV-FFMPEG.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = video_encoder

# Default target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
