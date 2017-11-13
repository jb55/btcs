
CFLAGS=-g -O0 -Ideps -std=c99 -Wall -Wno-unused-variable -Wno-unused-function -Wunreachable-code

DEPS=script.c \
     oplookup.c \
     op.c \
     val.c \
     alloc.c \
     script_num.c \
     stack.c

CLIDEPS=parser.tab.c \
				lex.yy.c \
        main.c

TESTDEPS=test.c

# DEPS=oplookup.h script.h misc.h Makefile op.h stack.h
TESTDEPS+=$(wildcard deps/*/*.c)
OBJS=$(DEPS:.c=.o)
CLIOBJS=$(CLIDEPS:.c=.o)
TESTOBJS=$(TESTDEPS:.c=.o)

GEN=parser.tab.c \
    parser.tab.h \
    lex.yy.c \
    oplookup.c \
    oplookup.h \
    $(OBJS) \
    $(CLIOBJS) \
    $(TESTOBJS) \


PREFIX ?= /usr/local
BIN=btcs

all: $(BIN)

include $(OBJS:.o=.d)
include $(CLIOBJS:.o=.d)
include $(TESTOBJS:.o=.d)

op.c: oplookup.h oplookup.c

%.d: %.c
	@rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@

oplookup.c oplookup.h: opcodes mph-opcodes
	@./mph-opcodes opcodes

parser.tab.c parser.tab.h:	parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

install: $(BIN)
	mkdir -p $(PREFIX)/bin
	cp $(BIN) $(PREFIX)/bin

$(BIN): $(OBJS) $(CLIOBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(CLIOBJS)

run_tests: $(OBJS) $(TESTOBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(TESTOBJS)

test: run_tests
	@./run_tests

clean:
	rm -f $(GEN)
	rm -f *.d

TAGS:
	etags -o - *.c > $@

.PHONY: TAGS test clean
