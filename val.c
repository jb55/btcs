
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

struct val
val_from_int(s64 intval) {
  struct val val;
  struct num *sn;
  int ind;
  sn = num_pool_new(&ind);
  val.type = VT_SCRIPTNUM;
  val.ind = ind;
  sn->ind = ind;
  sn->val = intval;
  return val;
}

void
val_serialize(struct val val, u16 *len, u8 *buf, int bufsize) {
  struct num *sn;
  int n;
  u16 valsize;
  switch (val.type) {
  case VT_SCRIPTNUM:
    sn = num_pool_get(val.ind);
    assert(sn);
    sn_serialize(sn, buf+1, bufsize - 1, &valsize);
    assert(valsize <= 0xFF);
    *buf = (u8)valsize;
    *len = valsize + 1;
    return;
  case VT_DATA:
    assert("!implement val_serialize VT_DATA");
    byte_pool_get(val.ind, len);
  case VT_SMALLINT:
    *len = 1;
    n = val.ind;
    if (n >= -1 && n <= 16) {
      val.type = VT_SMALLINT;
      assert(bufsize >= 1);
      *buf = intbytes[n+1];
    }
    else {
      assert(!"non-small VT_SMALLINT");
    }
  }

  assert(!"val_serialize missing implementation");
}

int
val_eq(struct val a, struct val b, int require_minimal) {
  u16 alen, blen;
  int eq = 0;

  static const int tmpsize = 4096;
  static u8 tmpa[tmpsize];
  static u8 tmpb[tmpsize];

  const u8 *abytes, *bbytes;
  // TODO: do I need to serialize to compare?
  /* abytes = val_serialize(a, &alen, require_minimal); */
  /* bbytes = val_serialize(b, &blen, require_minimal); */
  val_serialize(a, &alen, tmpa, tmpsize);
  val_serialize(b, &blen, tmpb, tmpsize);
  // TODO: what if they're semantically equivalent? or does that matter
  // (eg. minimal vs not miniminal)?

  if (alen != blen)
    return 0;

  eq = memcmp(tmpa, tmpb, alen) == 0;

  return eq;
}


/* struct val */
/* val_int(s64 n) { */
/*   struct val val; */
/* } */
