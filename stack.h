
#ifndef BTCS_STACK_H
#define BTCS_STACK_H

#include <assert.h>
#include "misc.h"

// Maximum number of values on script interpreter stack
static const int MAX_STACK_SIZE = 1000;

// Maximum number of values on script interpreter stack
static const int DEFAULT_STACK_SIZE = 64;

struct stack {
  u8 _data[DEFAULT_STACK_SIZE];
  u8 * bottom;
  u8 * top;
  int capacity;
};

int  stack_init(struct stack *);
int  stack_init_size(struct stack *, int size);
void stack_free(struct stack *);
void stack_clear(struct stack *);
void stack_push(struct stack *, u8 val);

static inline u8
stack_top(struct stack *stack, int ind) {
  u8 *p = stack->top + ind;
  assert(p >= stack->bottom);
  return *p;
}

static inline int
stack_size(struct stack *stack) {
  return stack->top - stack->bottom;
}

#endif /* BTCS_STACK_H */
