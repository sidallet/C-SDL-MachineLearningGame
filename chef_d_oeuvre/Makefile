
SRC=$(wildcard *.c) 
EXE=carcheez.out

CC=gcc

CFLAGS:=-std=c11 -Wall -Wextra -MMD $(sdl2-config --cflags)

BUILD=release

LDFLAGS:=-lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2 -pthread

ifeq ($(BUILD), debug)
	CFLAGS:=$(CFLAGS) -Og -g #-fsanitize=undefined,address
	LDFLAGS:=$(LDFLAGS) -g #-fsanitize=undefined,address
else
	ifeq ($(BUILD), profil)	
		LDFLAGS:=$(LDFLAGS) -pg #-fsanitize=undefined
		CFLAGS:=$(CFLAGS) -Og -pg #-fsanitize=undefined
	else
		CFLAGS:=$(CFLAGS) -O2
	endif
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
