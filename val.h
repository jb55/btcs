
#ifndef BTCS_VAL_H
#define BTCS_VAL_H

#include "misc.h"
#include "stack.h"

enum valtype {
  VT_SCRIPTNUM=0,
  VT_SMALLINT,
  VT_DATA,
  VT_N
};
// UPDATE VAL_TYPE_BITS if you need more valtypes


#define VAL_TYPE_BITS 2
/* static const int COMPACT_VAL_BITS = (32-VAL_TYPE_BITS-1); */
#define VAL_COMPACT_BITS 30

struct val {
  u8 type : VAL_TYPE_BITS;
  int ind : VAL_COMPACT_BITS;
};

#define smallintval(n) ((struct val){ .type = VT_SMALLINT, .ind = n })

// we want val to fit into the size of a 32-bit pointer
STATIC_ASSERT(sizeof(struct val) <= 4, val_doesnt_fit_in_stack);


static inline struct val
stack_top_val(struct stack *stack, int ind) {
  struct val val;
  stack_top_small(struct val, stack, &val, ind);
  return val;
}

static inline void
stack_set_val(struct stack *stack, int ind, struct val val) {
  struct val *pval = (struct val *)(stack->top + ind);
  *pval = val;
}

int
val_eq(struct val a, struct val b, int require_minimal);

#endif /* BTCS_VAL_H */
