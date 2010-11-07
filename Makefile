# Default C compiler options.
CFLAGS 		= -O2 -g -Wall

# C source files.
SOURCES 	= spor.c spor-ftw.c 

# Corresponding object files.
OBJECTS 	= $(SOURCES:.c=.o)

.PHONY: 	all clean

# Default target: build everything.
all: 	spor

# Clean up build products.
clean:
	rm -f $(OBJECTS) spor

spor: 	$(OBJECTS)
	$(CC) $(CFLAGS) -Wl -o $@ $^ 

$(OBJECTS): 	spor.c spor.h spor-ftw.c

