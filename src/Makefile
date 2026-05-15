CC = gcc

CFLAGS = -Wall -Wextra -g

EXEC = gal

SRC = main.c ast.c nfa.c dfa.c lecteur_exp.c

OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#pour tout netoyer a la fin
clean:
	rm -f $(OBJ) $(EXEC) *.dot *.png