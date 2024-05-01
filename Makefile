CFLAGS=-g -DHAVE_LITTLE_ENDIAN -O2 -Ideps -std=c99 -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-function 

DEPS=script.c \
     oplookup.c \
     op.c \
     val.c \
     alloc.c \
     script_num.c \
     sha256.c \
     ripemd160.c \
     compiler.c \
     util.c \
     stack.c

CLIDEPS=parser.tab.c \
	lex.yy.c \
	main.c

# DEPS=oplookup.h script.h misc.h Makefile op.h stack.h
OBJS=$(DEPS:.c=.o)
CLIOBJS=$(CLIDEPS:.c=.o)

GEN=parser.tab.c \
    parser.tab.h \
    lex.yy.c \
    oplookup.c \
    oplookup.h \
    $(OBJS) \
    $(CLIOBJS) \


PREFIX ?= /usr/local
BIN=btcs

all: $(BIN)

include $(OBJS:.o=.d)
include $(CLIOBJS:.o=.d)

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
	mkdir -p $(PREFIX)/share/btcs
	cp $(BIN) $(PREFIX)/bin
	cp btcs.1 $(PREFIX)/share/btcs

$(BIN): $(OBJS) $(CLIOBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(CLIOBJS)


check: $(BIN) fake
	@sh -c "cd test && ./run"

clean: fake
	rm -f $(GEN)
	rm -f *.d

TAGS: fake
	etags -o - *.c > $@

tags: fake
	ctags *.c

.PHONY: fake
