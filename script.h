
#ifndef BTCS_SCRIPT_H
#define BTCS_SCRIPT_H

#include "stack.h"
#include "op.h"

struct result {
  char * error;
  enum opcode last_op;
  int op_count;
};

int   script_eval(const u8 *script, size_t script_size, struct stack *stack,
                  struct result *result);
void  script_print_ops(char * buf, size_t size);
void  script_print_vals(struct stack *);
void  script_push_int(struct stack *, s64);

void  script_push_str(struct stack *script, char *str);
void  script_push_datastr(struct stack *, const char *str);

void
script_serialize(struct stack *stack, u8 *buf, int buflen, int* len);

extern int g_silence_script_err;
extern int g_silence_script_warn;

#endif /* BTCS_SCRIPT_H */
