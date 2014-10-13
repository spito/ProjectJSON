ELF=jParser
CC=g++
CXXFLAGS=-std=c++11

CXXFLAGS_DEBUG=-ggdb3
CXXFLAGS_RELEASE=-O2

DIR=ProjectJSON/
DIR_JSON=$(DIR)json/
DIR_DEBUG=$(DIR)debug/
DIR_PARSER=$(DIR_JSON)parser/

HEADERS_OBJECTS = $(DIR_JSON)objects/*.h
HEADERS_EXCEPTIONS = $(DIR_JSON)exception/*.h
HEADERS = $(DIR)*.h $(DIR_JSON)*.h $(DIR_PARSER)*.h $(HEADERS_EXCEPTIONS) $(HEADERS_OBJECTS)


debug: $(DIR_DEBUG) tests $(ELF)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(DIR_DEBUG)*.o -o $(ELF)

tests: json_debug $(DIR_DEBUG)tests.o

$(DIR_DEBUG)tests.o: $(DIR)tests.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)tests.o $(DIR)tests.cpp

json_debug: $(DIR_DEBUG)json.o $(DIR_DEBUG)Parser.o $(DIR_DEBUG)Token.o $(DIR_DEBUG)Tokenizer.o $(DIR_DEBUG)main.o $(DIR_DEBUG)NumberParser.o

$(DIR_DEBUG)main.o: $(DIR)main.cpp $(DIR)tests.h
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)main.o $(DIR)main.cpp

$(DIR_DEBUG)json.o: $(DIR_JSON)json.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)json.o $(DIR_JSON)json.cpp

$(DIR_DEBUG)Parser.o: $(DIR_PARSER)Parser.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)Parser.o $(DIR_PARSER)Parser.cpp

$(DIR_DEBUG)Token.o: $(DIR_PARSER)Token.o $(HEADERS)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)Token.o $(DIR_PARSER)Token.cpp

$(DIR_DEBUG)Tokenizer.o: $(DIR_PARSER)Tokenizer.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)Tokenizer.o $(DIR_PARSER)Tokenizer.cpp
    
$(DIR_DEBUG)NumberParser.o: $(DIR_PARSER)NumberParser.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -c -o $(DIR_DEBUG)NumberParser.o $(DIR_PARSER)NumberParser.cpp

$(DIR_DEBUG):
	mkdir $(DIR_DEBUG)

clean:
	rm -rf $(DIR_DEBUG)*.o $(ELF)
