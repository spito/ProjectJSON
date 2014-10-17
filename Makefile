ELF=jParser
CXX=g++
CXXFLAGS=-std=c++11

CXXFLAGS_DEBUG=-ggdb3
CXXFLAGS_RELEASE=-O2

DIR=ProjectJSON/
DIR_JSON=$(DIR)json/
DIR_PARSER=$(DIR_JSON)parser/

HEADERS_OBJECTS:= $(wildcard $(DIR_JSON)objects/*.h)
HEADERS_EXCEPTIONS:= $(wildcard $(DIR_JSON)exception/*.h)
HEADERS:= $(wildcard $(DIR)*.h) $(wildcard $(DIR_JSON)*.h) $(wildcard $(DIR_PARSER)*.h) $(HEADERS_EXCEPTIONS) $(HEADERS_OBJECTS)

SOURCES:=$(wildcard $(DIR)*.cpp) $(wildcard $(DIR_JSON)*.cpp) $(wildcard $(DIR_PARSER)*.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)

.PHONY: all
all: debug

.PHONY: debug 
debug: CXXFLAGS+=$(CXXFLAGS_DEBUG)
debug: $(ELF)

.PHONY: release 
release: CXXFLAGS+=$(CXXFLAGS_RELEASE) 
release: $(ELF)

$(ELF): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(ELF)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS)*.o $(ELF)
