BUILD_SUBDIRS = yacc
GRAMMAR_SOURCES = src/grammar_main.cpp src/grammar.cpp 
PARSER_SOURCES = src/parser_main.cpp src/parser.cpp
GRAMMAR_OBJECTS = $(GRAMMAR_SOURCES:.cpp=.o)
PARSER_OBJECTS = $(PARSER_SOURCES:.cpp=.o)
GRAMMAR_EXECUTABLE = grammarparser
PARSER_EXECUTABLE = parser
YACC_OBJECTS = yacc/yacc_main.o yacc/lex.yy.o yacc/y.tab.o
YACC_EXECUTABLE = yaccparser

program_INCLUDE_DIRS :=
program_LIBRARY_DIRS :=
program_LIBRARIES :=

CC = g++
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))


.PHONY: all clean distclean grammar_exeuytable parser_executable yacc_executable

all: grammar_executable parser_executable yacc_executable

grammar_executable: $(GRAMMAR_EXECUTABLE)

parser_executable: $(PARSER_EXECUTABLE)

yacc_executable: $(YACC_EXECUTABLE)

$(GRAMMAR_EXECUTABLE): $(GRAMMAR_OBJECTS)
	$(CC) $(GRAMMAR_OBJECTS) -o $(GRAMMAR_EXECUTABLE)

$(PARSER_EXECUTABLE): $(PARSER_OBJECTS)
	$(CC) $(PARSER_OBJECTS) -o $(PARSER_EXECUTABLE)

$(YACC_EXECUTABLE): $(wildcard yacc/*)
	$(MAKE) -C yacc
	$(CC) $(YACC_OBJECTS) -o $(YACC_EXECUTABLE)

clean:
	@- $(RM) $(GRAMMAR_EXECUTABLE) $(PARSER_EXECUTABLE) $(YACC_EXECUTABLE)
	@- $(RM) $(GRAMMAR_OBJECTS) $(PARSER_OBJECTS)
	$(MAKE) -C yacc $@
distclean: clean