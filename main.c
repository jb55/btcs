
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "stack.h"
#include "script.h"
#include "alloc.h"
#include "util.h"

extern int yyparse();
extern FILE* yyin;

char * g_reader_buf;
char * g_reader_buf_top;
struct stack g_reader_stack;
u32 g_reader_buf_cap;
void yy_scan_string(const char *);


void yyerror(const char* s);

#define streq(a,b) strcmp(a,b) == 0


void parse_error(char* err) {
  fprintf(stderr, "[btcs] parse error: %s\n", err);
  exit(1);
}

#define COMPILE_SHOW_SCRIPT      (1 << 0)
#define COMPILE_SHOW_SCRIPT_HEX  (1 << 1)
#define COMPILE_SHOW_STACK       (1 << 2)
#define COMPILE_SHOW_STACK_HEX   (1 << 3)
#define COMPILE_SHOW_RESULTS     (1 << 4)
#define COMPILE_SHOW_LABELS      (1 << 5)
#define COMPILE_SHOW_ALL         0x3F

static int compile(int compile_options, int argc, const char *argv[])
{
  yyin = stdin;

  struct result result;
  /* size_t size; */
  size_t bufsize = MAX_STACK_SIZE * MAX_STACK_SIZE;
  int i;
  int compiled_len;
  u8 *buf = (u8*)malloc(bufsize);
  struct stack tmp_stack;
  alloc_arenas(0, MAX_STACK_SIZE, MAX_STACK_SIZE * MAX_STACK_SIZE);
  stack_init(&tmp_stack);
  stack_init(&g_reader_stack);

  if (argc > 1) {
    for (i = 1; i < argc; ++i) {
      yy_scan_string(argv[i]);
      yyparse();
    }
  }
  else {
    do {
      yyparse();
    } while(!feof(yyin));
  }

  bool nol = (compile_options & COMPILE_SHOW_LABELS) == 0;

  /* size = g_reader_buf_top - g_reader_buf; */
  if (compile_options & COMPILE_SHOW_SCRIPT) {
	  if (compile_options & COMPILE_SHOW_LABELS)
		printf("script      ");
	  script_print_vals(&g_reader_stack);
  }

  script_serialize(&g_reader_stack, buf, bufsize, &compiled_len);
  script_eval(buf, compiled_len, &tmp_stack, &result);

  if (compile_options & COMPILE_SHOW_SCRIPT_HEX) {
	  if (compile_options & COMPILE_SHOW_LABELS)
		printf("script_hex  ");
	  for(i = 0; i < compiled_len; ++i)
		  printf("%02x", buf[i]);
	  printf("\n");
  }

  if (compile_options & COMPILE_SHOW_STACK) {
	  if (compile_options & COMPILE_SHOW_LABELS)
		printf("stack       ");
	  script_print_vals(&tmp_stack);
  }

  stack_serialize(&tmp_stack, buf, bufsize, &compiled_len);

  if (compile_options & COMPILE_SHOW_STACK_HEX) {
	  if (compile_options & COMPILE_SHOW_LABELS)
		printf("stack_hex   ");
	  for(i = 0; i < compiled_len; ++i)
		  printf("%02x", buf[i]);
	  printf("\n");
  }

  if (compile_options & COMPILE_SHOW_RESULTS) {
	  if (compile_options & COMPILE_SHOW_LABELS)
		printf("results    ");
	  if (result.error)
		  printf(" error:%d:%s:%s", result.op_count, result.error,
			 op_name(result.last_op));
	  else
		  printf(" success");
	  printf("\n");
  }

  stack_free(&g_reader_stack);
  stack_free(&tmp_stack);
  free_arenas(0);

  return !!result.error;
}

static void fail(int err, const char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
	exit(err);
}


static int decompile(const char *str, int strlen)
{
	static u8 buf[10000];

	if (strlen % 2 != 0)
		return 0;

	hex_decode(str, strlen, buf, sizeof(buf));
	size_t nbytes = strlen / 2;

	script_print(buf, nbytes);

	return 1;
}


static void usage()
{
	fprintf(stderr, "usage: btcs [OPTIONS] <script>\n\n");
	fprintf(stderr, "   OPTIONS\n\n");
	fprintf(stderr, "      -d,--decompile    decompile a base16 string to bitcoin script\n");
	exit(1);
}

int main(int argc, const char *argv[])
{
	static u8 buf[20000];

	bool is_decompile = false;
	const char *input = NULL;
	size_t written;
	int compile_options = 0;
	int last_opt = 0;
	bool hide_labels = false;

	if (argc == 2 && (streq(argv[1], "-h") || streq(argv[1], "--help")))
		usage();

	for (int i = 1; i < argc; i++) {
		if (streq(argv[i], "-d") || streq(argv[i], "--decompile"))
			is_decompile = true;
		else if (streq(argv[i], "+sh"))
			compile_options |= COMPILE_SHOW_SCRIPT_HEX;
		else if (streq(argv[i], "+s"))
			compile_options |= COMPILE_SHOW_SCRIPT_HEX;
		else if (streq(argv[i], "+st"))
			compile_options |= COMPILE_SHOW_STACK;
		else if (streq(argv[i], "+sth"))
			compile_options |= COMPILE_SHOW_STACK_HEX;
		else if (streq(argv[i], "-q") || streq(argv[i], "--hide-labels"))
			hide_labels = true;
		else {
			last_opt = i-1;
			input = argv[i];
			break;
		}
	}

	if (is_decompile) {
		int ok = read_arg_or_stdin(input, buf, sizeof(buf), &written);

		if (!ok)
			fail(3, "failed to read file or stdin");

		ok = decompile((const char *)buf, written);

		if (!ok)
			fail(4, "failed to decompile");
	}
	else {
		if (!compile_options)
			compile_options = COMPILE_SHOW_ALL;

		if (hide_labels)
			compile_options &= ~COMPILE_SHOW_LABELS;

		exit( compile(compile_options, argc - last_opt, argv + last_opt) );
	}
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}
