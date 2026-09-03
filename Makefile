CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -Iinc -g -pthread

BLD = build
TRG = $(BLD)/game
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=$(BLD)/%.o)

APP_OBJ = $(OBJ) $(BLD)/main.o

TST_TRG = $(BLD)/test_runner
TST_SRC = $(wildcard tests/*.c)
TST_OBJ = $(TST_SRC:%.c=$(BLD)/%.o)

.PHONY: all build run test clean

all: $(TRG)

build: $(APP_OBJ)

run: $(TRG)
	@./$(TRG)

$(TRG): $(APP_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TST_TRG)
	@./$(TST_TRG)

$(TST_TRG): $(TST_OBJ) $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BLD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BLD)
