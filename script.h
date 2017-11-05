
#ifndef BTCS_SCRIPT_H
#define BTCS_SCRIPT_H

#include "stack.h"

struct script {
  struct stack data;
  struct stack pushdata; // a stack of pushdata stacks
};

int   script_new(struct script *);
void  script_free(struct script *);
int   script_eval(u8 *script, size_t script_size, struct stack *stack);
void  script_print_ops(char * buf, size_t size);
void  script_print_vals(struct stack *);
void  script_push_int(struct stack *, s64);

void
script_serialize(struct stack *stack, u8 *buf, int buflen, int* len);

extern int g_silence_script_err;
extern int g_silence_script_warn;

#endif /* BTCS_SCRIPT_H */
