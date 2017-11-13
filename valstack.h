
#ifndef BTCS_VALSTACK_H
#define BTCS_VALSTACK_H

#include "op.h"
#include "val.h"

static inline struct val
stack_top_val(struct stack *stack, int ind) {
  struct val val;
  stack_top_small(struct val, stack, &val, ind);
  return val;
}

static inline struct val
stack_pop_val(struct stack *stack) {
  struct val val;
  val = stack_top_val(stack, -1);
  stack_pop(stack);
  return val;
}

static inline void
stack_set_val(struct stack *stack, int ind, struct val val) {
  struct val *pval = (struct val *)(stack->top + ind);
  *pval = val;
}

static inline void
stack_push_val(struct stack *stack, struct val val) {
#if 0
  printf("pushing val ");
  val_print(val);
  printf("\n");
#endif
  stack_push_small(struct val, stack, &val);
}

static inline void
stack_push_data(struct stack *stack, u8 *data, int len) {
  struct val val = { .type = VT_DATA };
  u8 *p;
  u16 ind;
  p = byte_pool_new(len, &ind);
  memcpy(p, data, len);
  val.ind = ind;
  stack_push_val(stack, val);
}

static inline void
stack_push_op(struct stack *stack, enum opcode opcode) {
  struct val val = { .type = VT_OP, .ind = opcode };
  stack_push_val(stack, val);
}

#endif /* BTCS_VALSTACK_H */
