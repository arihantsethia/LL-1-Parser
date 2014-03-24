LEX_FILE = lex.l
YACC_FILE = yacc.y
YACC_SOURCES = yacc_main.cpp 
GENERATED_SOURCES = lex.yy.c y.tab.c
GENERATED_HEADERS = y.tab.h
YACC_OBJECTS = yacc_main.o lex.yy.o y.tab.o

CC = g++ 
LL = flex
YC = yacc

.PHONY: all

all: $(LEX) $(YACC)

$(LEX): $(LEX_FILE)
	$(LL) $(LEX_FILE)

$(YACC): $(YACC_FILE)
	$(YC) -d $(YACC_FILE)
	$(CC) -c -w $(YACC_SOURCES) $(GENERATED_SOURCES)

clean:
	@- $(RM) $(YACC_OBJECTS) $(GENERATED_SOURCES) $(GENERATED_HEADERS)