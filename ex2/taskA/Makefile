# program executable name
TARGET = main

# compiler flags
CFLAGS = -g -Wall -std=gnu11

# linker flags
LDFLAGS = -g

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.0)

# link programs
$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^

# compile
%.0 : %.c
	gcc $(CFLAGS) -c -o $@ $<

# clean

clean:
	rm -f $(TARGET) $(OBJECTS)


