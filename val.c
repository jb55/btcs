
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
  u16 ind;
  sn = num_pool_new(&ind);
  val.type = VT_SCRIPTNUM;
  val.ind = ind;
  sn->ind = ind;
  sn->val = intval;
  return val;
}

// TODO maybe replace this with serialize
void val_bytes(struct val val, u32 *len, u8 *buf, int bufsize) {
  if (val.type == VT_DATA)
    val.type = VT_RAW;
  static const int serialize_minimal = 0;
  val_serialize(val, len, buf, bufsize, serialize_minimal);
}

void val_serialize(struct val val, u32 *len, u8 *buf, int bufsize, int serialize_minimal) {
  struct num *sn;
  int n;
  u16 valsize;
  switch (val.type) {
  case VT_SCRIPTNUM:
    sn = num_pool_get(val.ind);
    assert(sn);

    if (serialize_minimal) {
      if (sn->val == -1) { *len = 1; *buf = OP_1NEGATE; return; }
      if (sn->val == 0 ) { *len = 1; *buf = 0; return; }
      if (sn->val >= 1 && sn->val <= 16 ) {
        *len = 1;
        *buf = OP_1 - 1 + sn->val;
        return;
      }
    }
    sn_serialize(sn, buf, bufsize, &valsize);
    assert(valsize <= 0xFF);
    *len = valsize;
    return;
  case VT_OP:
    *len = 1;
    *buf = val.ind & 0xFF;
    return;
  case VT_RAW: {
    u8 *p;
    p = byte_pool_get(val.ind, len);
    memcpy(buf, p, *len);
    return;
  }
  case VT_DATA: {
    u8 *p;
    p = byte_pool_get(val.ind, len);
    if (*len < OP_PUSHDATA1) {
      *buf++ = *len;
      memcpy(buf, p, *len);
      *len += 1;
    }
    else if (*len <= 0xFF) {
      *buf++ = OP_PUSHDATA1;
      *buf++ = *len;
      memcpy(buf, p, *len);
      *len += 2;
    }
    else if (*len <= 0xFFFF) {
      *buf++ = OP_PUSHDATA2;
      u16 *sp = (u16*)buf;
      // TODO: writele16
      *sp = *len;
      buf += 2;
      memcpy(buf, p, *len);
      *len += 3;
    }
    else {
      *buf++ = OP_PUSHDATA4;
      u32 *ip = (u32*)buf;
      // TODO: writele32
      *ip = *len;
      buf += 4;
      memcpy(buf, p, *len);
      *len += 5;
    }
    return;
  }
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
    return;
  }

  assert(!"val_serialize missing implementation");
}

int
val_eq(struct val a, struct val b, int require_minimal) {
  u32 alen = 0, blen = 0;
  int eq = 0;

  static const int tmpsize = 4096;
  static u8 tmpa[4096];
  static u8 tmpb[4096];

  // TODO: do I need to serialize to compare?
  /* abytes = val_serialize(a, &alen, require_minimal); */
  /* bbytes = val_serialize(b, &blen, require_minimal); */
  val_serialize(a, &alen, tmpa, tmpsize, 0);
  val_serialize(b, &blen, tmpb, tmpsize, 0);
  // TODO: what if they're semantically equivalent? or does that matter
  // (eg. minimal vs not miniminal)?

  if (alen != blen)
    return 0;

  eq = memcmp(tmpa, tmpb, alen) == 0;

  return eq;
}

struct val
val_copy(struct val a) {
  u32 len;
  u16 newind;
  u8 *src, *dst;
  struct num *nsrc, *ndst;

  switch (a.type) {
  case VT_SMALLINT:
    return a;
  case VT_DATA:
    src = byte_pool_get(a.ind, &len);
    dst = byte_pool_new(len, &newind);
    memcpy(dst, src, len);
    a.ind = newind;
    return a;
  case VT_SCRIPTNUM:
    nsrc = num_pool_get(a.ind);
    ndst = num_pool_new(&newind);
    ndst->val = nsrc->val;
    return sn_to_val(ndst);
  default:
    assert(!"unhandle val_copy enum");
  }
}

u32
val_size(struct val val) {
  u32 len;

  switch (val.type) {
  case VT_DATA: {
    byte_pool_get(val.ind, &len);
    return len;
  }
  default:
    return 1;
  }
}

/* struct val */
/* val_int(s64 n) { */
/*   struct val val; */
/* } */
