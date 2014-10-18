ELF=jParser
ELF_TEST=jParserTest
CXX=g++
CXXFLAGS=-std=c++11

CXXFLAGS_DEBUG=-ggdb3
CXXFLAGS_RELEASE=-O2

DIR=ProjectJSON/
DIR_JSON=$(DIR)json/
DIR_PARSER=$(DIR_JSON)parser/
DIR_TESTS=$(DIR)tests/

INCLUDE=$(DIR)

HEADERS_OBJECTS:= $(wildcard $(DIR_JSON)objects/*.h)
HEADERS_EXCEPTIONS:= $(wildcard $(DIR_JSON)exception/*.h)
HEADERS:= $(wildcard $(DIR)*.h) $(wildcard $(DIR_JSON)*.h) $(wildcard $(DIR_TESTS)*.h) $(wildcard $(DIR_PARSER)*.h) $(HEADERS_EXCEPTIONS) $(HEADERS_OBJECTS)

SOURCES:=$(wildcard $(DIR_JSON)*.cpp) $(wildcard $(DIR_PARSER)*.cpp)
SOURCES_MAIN:=$(SOURCES) $(DIR)main.cpp
SOURCES_TEST:=$(SOURCES) $(wildcard $(DIR_TESTS)*.cpp)
OBJECTS_MAIN:=$(SOURCES_MAIN:.cpp=.o)
OBJECTS_TEST:=$(SOURCES_TEST:.cpp=.o)

.PHONY: all
all: debug

.PHONY: test
test: debug
	./$(ELF_TEST)

.PHONY: debug
debug: CXXFLAGS+=$(CXXFLAGS_DEBUG) -I$(INCLUDE)
debug: $(ELF) $(ELF_TEST)

.PHONY: release
release: CXXFLAGS+=$(CXXFLAGS_RELEASE) -I$(INCLUDE)
release: $(ELF) $(ELF_TEST)

$(ELF): $(OBJECTS_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $(ELF)

$(ELF_TEST): $(OBJECTS_TEST)
	$(CXX) $(CXXFLAGS) $^ -o $(ELF_TEST)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS_MAIN) $(OBJECTS_TEST) $(ELF) $(ELF_TEST)
