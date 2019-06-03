
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "script.h"
#include "alloc.h"

extern int yyparse();
extern FILE* yyin;

char * g_reader_buf;
char * g_reader_buf_top;
struct stack g_reader_stack;
u32 g_reader_buf_cap;

void yyerror(const char* s);


void
parse_error(char* err) {
  fprintf(stderr, "[btcs] parse error: %s\n", err);
  exit(1);
}

int main(int argc, const char *argv[]) {
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
    for (int i = 1; i < argc; ++i) {
      yy_scan_string(argv[i]);
      yyparse();
    }
  }
  else {
    do {
      yyparse();
    } while(!feof(yyin));
  }

  /* size = g_reader_buf_top - g_reader_buf; */
  printf("script     ");
  script_print_vals(&g_reader_stack);
  script_serialize(&g_reader_stack, buf, bufsize, &compiled_len);
  script_eval(buf, compiled_len, &tmp_stack, &result);

  printf("script_hex  ");
  for(i = 0; i < compiled_len; ++i)
    printf("%02x", buf[i]);
  printf("\n");

  printf("output     ");
  script_print_vals(&tmp_stack);
  stack_serialize(&tmp_stack, buf, bufsize, &compiled_len);

  printf("output_hex  ");
  for(i = 0; i < compiled_len; ++i)
    printf("%02x", buf[i]);
  printf("\n");

  printf("results    ");
  if (result.error) printf(" error:%d:%s:%s", result.op_count, result.error,
                           op_name(result.last_op));
  else printf(" success");
  printf("\n");

  stack_free(&g_reader_stack);
  stack_free(&tmp_stack);
  free_arenas(0);

  return !!result.error;
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}
