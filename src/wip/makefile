CC = gcc
CFLAGS = -Wall -Wextra -I./cJSON
OBJ = addpath.o
EXE = AddPath

all: $(EXE)

$(EXE): $(OBJ) cJSON.o
	$(CC) -o $(EXE) $(OBJ) cJSON.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del $(OBJ) $(EXE)
