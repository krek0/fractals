# Makefile

CC = gcc
CFLAGS = -Wall -lm -Ofast `sdl2-config --libs --cflags` -lSDL2_image
SRC = fractals.c img.c main.c debug.c
OBJ = $(SRC:%.c=build/%.o)
EXEC = build/out

# Default build
all: $(EXEC)

# Debug build
debug: CFLAGS += -DDEBUG
debug: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

build/%.o: %.c | build
	$(CC) -c -o $@ $< $(CFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build/*.o

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run debug

