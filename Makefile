
CFLAGS=-O2 -Wall -Wno-unused-variable

FORBIN=script.o \
       parser.tab.o \
       lex.yy.o \
       oplookup.o \
       op.o \
       main.o \
       stack.o

GEN=parser.tab.c parser.tab.h lex.yy.c oplookup.c oplookup.h $(FORBIN)

DEPS=oplookup.h script.h misc.h Makefile op.h stack.h

PREFIX ?= /usr/local
BIN=btcs


all: $(BIN)

oplookup.c oplookup.h: opcodes mph-opcodes
	@./mph-opcodes opcodes

parser.tab.c parser.tab.h:	parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

install: $(BIN)
	mkdir -p $(PREFIX)/bin
	cp $(BIN) $(PREFIX)/bin

$(BIN): $(GEN) $(DEPS) $(FORBIN)
	$(CC) $(CFLAGS) -o $@ $(FORBIN)

clean:
	rm -f $(GEN)
