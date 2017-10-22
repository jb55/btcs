

GEN=parser.tab.c parser.tab.h lex.yy.c oplookup.c
DEPS=script.c parser.tab.c lex.yy.c oplookup.c
PREFIX ?= /usr/local
BIN=bcs

all: $(BIN)

oplookup.c: opcodes
	@./mph-opcodes opcodes > $@

parser.tab.c parser.tab.h:	parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

install: $(BIN)
	mkdir -p $(PREFIX)/bin
	cp $(BIN) $(PREFIX)/bin

$(BIN): $(GEN) $(DEPS)
	$(CC) -O0 -g -o $@ $(DEPS)

clean:
	rm -f $(GEN)
