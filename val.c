
#include <assert.h>
#include "op.h"
#include "val.h"
#include "alloc.h"
#include "script_num.h"

static const char intbytes[] = {
  OP_1NEGATE,
  OP_0,
  OP_1, OP_2, OP_3, OP_4, OP_5,
  OP_6, OP_7, OP_8, OP_9, OP_10,
  OP_11, OP_12, OP_13, OP_14, OP_15,
  OP_16
};

const char *
val_serialize(struct val val, u16 *len, int require_minimal) {
  struct num *sn;
  int n, ind;
  switch (val.type) {
  case VT_SCRIPTNUM:
    sn = num_pool_get(val.ind);
    assert(sn);
    return sn_serialize(sn, len);
  case VT_DATA:
    return byte_pool_get(val.ind, len);
  case VT_SMALLINT:
    *len = 1;
    n = val.ind;
    if (n >= -1 && n <= 16) {
      val.type = VT_SMALLINT;
      return &intbytes[n+1];
    }
    else {
      assert(!"non-small VT_SMALLINT");
    }
  }

  assert(!"val_serialize missing implementation");
  return 0;
}

int
val_eq(struct val a, struct val b, int require_minimal) {
  u16 alen, blen;
  int eq = 0;
  const char *abytes, *bbytes;
  abytes = val_serialize(a, &alen, require_minimal);
  bbytes = val_serialize(b, &blen, require_minimal);
  // TODO: what if they're semantically equivalent? or does that matter?

  if (alen != blen) {
    if (a.type == VT_DATA) byte_pool_pop();
    if (b.type == VT_DATA) byte_pool_pop();
    return 0;
  }

  eq = memcmp(abytes, bbytes, alen) == 0;

  if (a.type == VT_DATA) byte_pool_pop();
  if (b.type == VT_DATA) byte_pool_pop();

  return eq;
}


/* struct val */
/* val_int(s64 n) { */
/*   struct val val; */
/* } */
