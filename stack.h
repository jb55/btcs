
#ifndef BTCS_STACK_H
#define BTCS_STACK_H

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "misc.h"

// Maximum number of values on script interpreter stack
static const int MAX_STACK_SIZE = 1000;

// Maximum number of values on script interpreter stack
#define DEFAULT_STACK_SIZE 32

struct stack {
  void *_data[DEFAULT_STACK_SIZE];
  void **bottom;
  void **top;
  int capacity;
};

int   stack_init(struct stack *);
int   stack_init_size(struct stack *, int size);
void  stack_free(struct stack *);
void  stack_clear(struct stack *);
void* stack_pop(struct stack *);
void* stack_erase(struct stack *, int ind);
void  stack_push(struct stack *, void *val);

static inline void *
stack_top(struct stack *stack, int ind) {
  void **p = stack->top + ind;
  assert(p >= stack->bottom);
  assert(p);
  return *p;
}

static inline int
stack_size(struct stack *stack) {
  return stack->top - stack->bottom;
}

static inline void
stack_swap(struct stack *stack, int ind1, int ind2) {
  void *temp = *(stack->top + ind1);
  *(stack->top + ind1) = *(stack->top + ind2);
  *(stack->top + ind2) = temp;
}

#define stack_push_small(T, stack, val) { \
  assert(sizeof(T) <= sizeof(void*)); \
  void *tmp = 0; \
  memcpy(&tmp, val, sizeof(T));                    \
  stack_push(stack, tmp); \
}

#define stack_top_small(T, stack, v, ind)     \
  memcpy(v, stack->top + ind, sizeof(T))

static inline void
_stack_push_smallt(struct stack *stack, void *p, size_t size) {
#if 0
  u8 *b = (u8*)p;
  printf("pushing small", "");
  for (size_t i = 0; i < size; ++i) {
    printf(" %02x", b[i]);
  }
  printf("\n");
#endif
  assert(size <= sizeof(void*));
  void *tmp = 0;
  memcpy(&tmp, p, size);
  stack_push(stack, tmp);
}

#endif /* BTCS_STACK_H */
