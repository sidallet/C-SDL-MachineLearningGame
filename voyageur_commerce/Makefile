
SRC=$(wildcard *.c)  # en commentaire, je ne suis pas un grand amateur
EXE=voyageur_commerce.out

CC=gcc

CFLAGS:=-std=c11 -Wall -Wextra -MMD $(sdl2-config --cflags)

BUILD=debug

#CFLAGS:=-Wall -Wextra -MMD -O2 $(sdl2-config --cflags)  # pour la version release
LDFLAGS:=-lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2

ifeq ($(BUILD), debug)
	CFLAGS:=$(CFLAGS) -Og -g #-fsanitize=address
	LDFLAGS:=$(LDFLAGS) #-fsanitize=address 
else
	CFLAGS:=$(CFLAGS) -O2
endif

OBJ=$(addprefix build/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))

all: $(OBJ) 
	$(CC) -o $(EXE) $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build core *.o

-include $(DEP)
