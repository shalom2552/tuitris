CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -Iinc -g -pthread

TRG = game
BLD = build
SRC = main.c $(wildcard src/*.c)
OBJ = $(SRC:%.c=$(BLD)/%.o)

.PHONY: all build run clean

all: $(TRG)

build: $(OBJ)

run: $(TRG)
	./$(TRG)

$(TRG): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BLD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BLD) $(TRG)
