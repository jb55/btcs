
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

void  script_push_str(struct stack *script, const char *str);
void  script_push_raw(struct stack *, const char *str);
void  script_push_datastr(struct stack *, const char *str, int israw);

void script_serialize(struct stack *stack, u8 *buf, int buflen, int* len);
void stack_serialize(struct stack *stack, u8 *buf, int buflen, int* len);

void script_print(const u8 *script, size_t script_size);
int script_getop(const u8 **p, const u8 *end, enum opcode *popcode, u8 *buf, int bufsize, u32 *outlen);

extern int g_silence_script_err;
extern int g_silence_script_warn;

#endif /* BTCS_SCRIPT_H */
