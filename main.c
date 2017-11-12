
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "script.h"
#include "alloc.h"

extern int yyparse();
extern FILE* yyin;

char * g_reader_buf;
char * g_reader_buf_top;
u32 g_reader_buf_cap;

void yyerror(const char* s);

int main() {
  yyin = stdin;

  size_t size;
  size_t bufsize = MAX_STACK_SIZE * MAX_STACK_SIZE;
  int compiled_len;
  u8 *buf = (u8*)malloc(bufsize);
  struct stack tmp_stack;
  alloc_arenas(0, MAX_STACK_SIZE, MAX_STACK_SIZE * MAX_STACK_SIZE);
  stack_init(&tmp_stack);

  do {
    yyparse();
  } while(!feof(yyin));

  size = g_reader_buf_top - g_reader_buf;
  script_serialize(g_reader_buf, buf, bufsize, &compiled_len);
  script_eval(buf, size, &tmp_stack);
  printf("script: ");
  script_print_ops(g_reader_buf, g_reader_buf_top);
  printf("stack:  ");
  script_print_vals(&tmp_stack);

  stack_free(&g_reader_buf);
  stack_free(&tmp_stack);
  free_arenas(0);

  return 0;
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}

