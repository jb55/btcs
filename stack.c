
#include "stack.h"
#include <stdlib.h>
#include <string.h>

void
stack_clear(struct stack *stack) {
  memset(stack->bottom, 0, stack->capacity);
}

int
stack_init_size(struct stack *stack, int capacity) {
  if (capacity > DEFAULT_STACK_SIZE) {
    u8 *bottom = (u8*)malloc(capacity);
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


void
stack_expand(struct stack *stack) {
  assert(!"Implement expand");
}

void
stack_push(struct stack *stack, u8 byte) {
  if ((stack_size(stack) + 1) > stack->capacity) {
    stack_expand(stack);
  }

  *stack->top++ = byte;
}

