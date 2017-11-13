
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

int main() {
  yyin = stdin;

  size_t size;
  size_t bufsize = MAX_STACK_SIZE * MAX_STACK_SIZE;
  int i;
  int compiled_len;
  u8 *buf = (u8*)malloc(bufsize);
  struct stack tmp_stack;
  alloc_arenas(0, MAX_STACK_SIZE, MAX_STACK_SIZE * MAX_STACK_SIZE);
  stack_init(&tmp_stack);
  stack_init(&g_reader_stack);

  do {
    yyparse();
  } while(!feof(yyin));

  size = g_reader_buf_top - g_reader_buf;
  printf("script     ");
  script_print_vals(&g_reader_stack);
  script_serialize(&g_reader_stack, buf, bufsize, &compiled_len);
  script_eval(buf, compiled_len, &tmp_stack);

  printf("script_hex  ");
  for(i = 0; i < compiled_len; ++i)
    printf("%02x", buf[i]);
  printf("\n");

  printf("stack      ");
  script_print_vals(&tmp_stack);
  script_serialize(&tmp_stack, buf, bufsize, &compiled_len);

  printf("stack_hex   ");
  for(i = 0; i < compiled_len; ++i)
    printf("%02x", buf[i]);
  printf("\n");

  stack_free(&g_reader_stack);
  stack_free(&tmp_stack);
  free_arenas(0);

  return 0;
}

void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}

