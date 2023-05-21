SOURCES.c = polynomials.c

OBJECTS = poly

CC = gcc

CFLAGS = -g -Wall -Werror
LDLIBS = -lm

all: $(OBJECTS)

$(OBJECTS): $(SOURCES.c)
	$(CC) $(CFLAGS) $(SOURCES.c) -o $(OBJECTS) $(LDLIBS)

clean: 
	rm $(OBJECTS)