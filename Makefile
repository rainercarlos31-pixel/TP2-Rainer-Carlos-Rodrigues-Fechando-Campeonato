CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
TARGET = campeonato
SRCS = main.c team.c teamlist_dlist.c match.c matchlist.c
OBJS = $(SRCS:.c=.o)

.PHONY: all compile run clean
all: compile
compile: $(TARGET)
run: $(TARGET)
	./$(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET) classificacao.csv
