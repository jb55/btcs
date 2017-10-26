
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void
stack_clear(struct stack *stack) {
  memset(stack->bottom, 0, stack->capacity);
}

int
stack_init_size(struct stack *stack, int capacity) {
  if (capacity > DEFAULT_STACK_SIZE) {
    void *bottom = malloc(capacity * sizeof(void*));
    if (!bottom) return 0;
    stack->bottom   = bottom;
    stack->top      = bottom;
    stack->capacity = capacity;
  }
  else {
    stack->bottom   = stack->_data;
    stack->top      = stack->bottom;
    stack->capacity = DEFAULT_STACK_SIZE;
  }
  stack_clear(stack);
  return 1;
}

int
stack_init(struct stack *stack) {
  return stack_init_size(stack, DEFAULT_STACK_SIZE);
}

void
stack_free(struct stack *stack) {
  if (stack->capacity > DEFAULT_STACK_SIZE)
    free(stack->bottom);
  stack->bottom = stack->_data;
}


int
stack_expand(struct stack *stack) {
  size_t newcap = stack->capacity * 2;
  void **bottom = 0;
  int size = stack_size(stack);

#if DEBUG
  printf("expanding s(%d) %d -> %d\n", size, stack->capacity, newcap);
#endif

  // XXX: might want to relax this to <= eventually
  assert(size == stack->capacity);

  if (stack->capacity <= DEFAULT_STACK_SIZE) {
    bottom = malloc(newcap * sizeof(void*));
    if (!bottom) return 0;
    memcpy(bottom, stack->bottom, size * sizeof(void*));
  }
  else {
    bottom = realloc(stack->bottom, newcap * sizeof(void*));
    if (!bottom) return 0;
  }
  stack->capacity = newcap;
  stack->bottom   = bottom;
  stack->top      = bottom+size;
  return 1;
}

void
stack_push(struct stack *stack, void *val) {
  #if 0
  printf("pushing: %d %p\n", stack_size(stack), val);
  #endif

  if ((stack_size(stack) + 1) > stack->capacity) {
    stack_expand(stack);
  }

  *stack->top++ = val;
}

// TODO: UNTESTED!
void *
stack_erase(struct stack *stack, int ind) {
  int size = stack_size(stack);
  assert(ind >= 0);
  assert(ind < size);
  void * erased = *(stack->bottom + ind);
  memcpy(stack->bottom + ind, stack->bottom + ind + size, size);
  stack->top--;
  return erased;
}

void *
stack_pop(struct stack *stack) {
  assert(stack_size(stack) != 0);
  void * v = stack_top(stack, -1);
  stack->top--;
  return v;
}

