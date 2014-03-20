GRAMMAR_SOURCES = src/grammar_main.cpp src/grammar.cpp 
PARSER_SOURCES = src/parser_main.cpp src/parser.cpp
GRAMMAR_OBJECTS = $(GRAMMAR_SOURCES:.cpp=.o)
PARSER_OBJECTS = $(PARSER_SOURCES:.cpp=.o)
GRAMMAR_EXECUTABLE = grammarparser	
PARSER_EXECUTABLE = parser

program_INCLUDE_DIRS :=
program_LIBRARY_DIRS :=
program_LIBRARIES :=

CC = g++
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean grammar parser

all: grammar_exectuable parser_exectuable

grammar_exectuable: $(GRAMMAR_EXECUTABLE)

parser_exectuable: $(PARSER_EXECUTABLE)

$(GRAMMAR_EXECUTABLE): $(GRAMMAR_OBJECTS)
	$(CC) $(GRAMMAR_OBJECTS) -o $(GRAMMAR_EXECUTABLE)

$(PARSER_EXECUTABLE): $(PARSER_OBJECTS)
	$(CC) $(PARSER_OBJECTS) -o $(PARSER_EXECUTABLE)

clean:
	@- $(RM) $(GRAMMAR_EXECUTABLE) $(PARSER_EXECUTABLE)
	@- $(RM) $(GRAMMAR_OBJECTS) $(PARSER_OBJECTS)

distclean: clean