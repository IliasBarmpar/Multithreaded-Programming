# set non-optional preprocessor flags here
# eg. project specific include directories
CPPFLAGS += -I ./include

# find cpp files in subdirectories
SOURCES := $(shell find . -name '*.cpp' -not -path "./main/*")

# find headers
HEADERS := $(shell find . -name '*.h')

OUTPUT1 := build/dropbox_server

OUTPUT2 := build/dropbox_client

# Everything depends on the output
all:	$(OUTPUT1) $(OUTPUT2)

# The output depends on sources and headers
$(OUTPUT1): $(SOURCES) main/server.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) -o $(OUTPUT1) $(SOURCES) main/server.cpp

$(OUTPUT2): $(SOURCES) main/client.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) -pthread -o $(OUTPUT2) $(SOURCES) main/client.cpp

.PHONY: clean

clean:
	rm -f $(OUTPUT1) $(OUTPUT2)

#all: dropbox_server dropbox_client

#dropbox_server:
#	g++ server.cpp list.cpp basicFunctions.cpp readCmd.cpp -o dropbox_server

#dropbox_client:
#	g++ -pthread client.cpp list2.cpp buffer.cpp readCmd.cpp threadFunctions.cpp basicFunctions.cpp -o dropbox_client

#.PHONY: dropbox_server dropbox_client

