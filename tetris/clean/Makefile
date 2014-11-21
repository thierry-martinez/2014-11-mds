SOURCES=tetris.c grid.c window.c
CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LDLIBS=$(shell pkg-config --libs gtk+-2.0)

all: tetris

clean:
	rm -f tetris $(SOURCES:.c=.o)

tetris: $(SOURCES:.c=.o)
