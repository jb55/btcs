
#ifndef BTCS_VAL_H
#define BTCS_VAL_H

#include "misc.h"
#include "stack.h"

enum valtype {
  VT_SCRIPTNUM=0,
  VT_SMALLINT,
  VT_OP,
  VT_RAW,
  VT_DATA,
  VT_N
};
// UPDATE VAL_TYPE_BITS if you need more valtypes


#define VAL_TYPE_BITS 3
/* static const int COMPACT_VAL_BITS = (32-VAL_TYPE_BITS-1); */
#define VAL_COMPACT_BITS 29

struct val {
  u8 type : VAL_TYPE_BITS;
  int ind : VAL_COMPACT_BITS;
};

#define smallintval(n) ((struct val){ .type = VT_SMALLINT, .ind = n })
#define falseval smallintval(0)
#define trueval  smallintval(1)

// we want val to fit into the size of a 32-bit pointer
STATIC_ASSERT(sizeof(struct val) <= 4, val_doesnt_fit_in_stack);

struct val val_copy(struct val a);
u32 val_size(struct val val);

int
val_eq(struct val a, struct val b);

void val_serialize(struct val val, u32 *len, u8 *buf, int bufsize);
void val_bytes(struct val val, u32 *len, u8 *buf, int bufsize);

struct val val_from_int(s64);

#endif /* BTCS_VAL_H */
