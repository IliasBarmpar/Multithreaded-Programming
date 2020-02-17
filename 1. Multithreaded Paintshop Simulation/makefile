CC := gcc
# set non-optional preprocessor flags here
# eg. project specific include directories
CPPFLAGS += -I ./include

# find cpp files in subdirectories
SOURCES := $(shell find . -name '*.c')

# find headers
HEADERS := $(shell find . -name '*.h')

OUTPUT := build/paintshop

# Everything depends on the output
all:	$(OUTPUT) 

# The output depends on sources and headers
$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) -o $(OUTPUT) $(SOURCES) 


.PHONY: clean

clean:
	rm -f $(OUTPUT)
