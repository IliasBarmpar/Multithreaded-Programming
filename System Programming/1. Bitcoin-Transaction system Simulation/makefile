# set non-optional preprocessor flags here
# eg. project specific include directories
CPPFLAGS += -I ./include

# find cpp files in subdirectories
SOURCES := $(shell find . -name '*.cpp')

# find headers
HEADERS := $(shell find . -name '*.h')

OUTPUT := build/bitcoin

# Everything depends on the output
all:	$(OUTPUT) 

# The output depends on sources and headers
$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CXX) $(CPPFLAGS) -o $(OUTPUT) $(SOURCES) 

.PHONY: clean

clean:
	rm -f $(OUTPUT)

