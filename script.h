
#ifndef BTCS_SCRIPT_H
#define BTCS_SCRIPT_H

#include "stack.h"

struct script {
  struct stack data;
  struct stack pushdata; // a stack of pushdata stacks
};

int  script_new(struct script *);
void script_free(struct script *);
int  script_eval(struct stack *, struct stack*);
void script_print_ops(struct stack *);
void script_print_vals(struct stack *);


#endif /* BTCS_SCRIPT_H */
