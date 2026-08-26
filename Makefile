CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -Iinc -g

TRG = game
SRC = main.c $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(TRG)

$(TRG): $(OBJ)
	$(CC) -o $(TRG) $(OBJ)

clean:
	rm -f $(OBJ) $(TRG)
