# program executable name
TARGET = main

# compiler flags
CFLAGS = -g -O3 -std=gnu11 -lpthread -fsanitize=thread

# linker flags
LDFLAGS = -g -fsanitize=thread

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.0)

# link programs
$(TARGET): $(OBJECTS)
	clang $(LDFLAGS) -o $@ $^

# compile
%.0 : %.c
	clang $(CFLAGS) -c -o $@ $<

# clean

clean:
	rm -f $(TARGET) $(OBJECTS)


