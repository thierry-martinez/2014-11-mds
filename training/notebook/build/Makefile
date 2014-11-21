SOURCES=gtk_example.c initialization.c
CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LDLIBS=$(shell pkg-config --libs gtk+-2.0)
    
all: gtk_example
    
clean:
	rm -f gtk_example $(SOURCES:.c=.o)
    
gtk_example: $(SOURCES:.c=.o)
