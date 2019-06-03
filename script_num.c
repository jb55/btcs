
#include "script_num.h"
#include "alloc.h"
#include "val.h"
#include <limits.h>
#include <inttypes.h>

/**
 * Numeric opcodes (OP_1ADD, etc) are restricted to operating on 4-byte
 * integers. The semantics are subtle, though: operands must be in the range
 * [-2^31 +1...2^31 -1], but results may overflow (and are valid as long as they
 * are not used in a subsequent numeric operation). struct num enforces those
 * semantics by storing results as an int64 and allowing out-of-range values to
 * be returned, but throwing an exception if arithmetic is done or the result is
 * interpreted as an integer.
 */


void
sn_from_int(s64 n, struct num *sn) {
  sn->ind = -1;
  sn->val = n;
}


/* enum sn_result */
/* sn_to_int(struct num *sn, int *res) { */
/*   if (sn_overflowed(sn)) { */
/*     return SN_ERR_OVERFLOWED_INT; */
/*   } */
/* } */


void sn_serialize(struct num *sn, u8 *buf, int bufsize, u16 *len) {
  u8 *p = buf;

  if(sn->val == 0) {
    *len = 0;
    return;
  }

  const int neg = sn->val < 0;
  u64 absvalue = neg ? -(sn->val) : sn->val;

  while(absvalue) {
    *(p++) = absvalue & 0xff;
    assert((p - buf) <= bufsize);
    absvalue >>= 8;
  }

  //    - If the most significant byte is >= 0x80 and the value is positive, push a
  //    new zero-byte to make the significant byte < 0x80 again.

  //    - If the most significant byte is >= 0x80 and the value is negative, push a
  //    new 0x80 byte that will be popped off when converting to an integral.

  //    - If the most significant byte is < 0x80 and the value is negative, add
  //    0x80 to it, since it will be subtracted and interpreted as a negative when
  //    converting to an integral.

  if (*(p - 1) & 0x80)
    *(p++) = neg ? 0x80 : 0;
  else if (neg)
    *(p - 1) |= 0x80;

  *len = p - buf;
}

static inline int
int_overflowed(s64 val) {
  return val < INT_MIN || val > INT_MAX;
}


int
sn_overflowed(struct num *num) {
  return int_overflowed(num->val);
}


s64
int_from_data(u8 *data, u16 size) {
  s64 result = 0;

  if (size == 0)
    return 0;

  for (size_t i = 0; i != size; ++i)
    result |= (s64)((data[i]) << 8*i);

  // If the input vector's most significant byte is 0x80, remove it from
  // the result's msb and return a negative.
  if (data[size-1] & 0x80) {
    return -((s64)(result & ~(0x80ULL << (8 * (size - 1)))));
  }

  return result;
}


enum sn_result
sn_from_data(u8 *data, u16 size, struct num **num) {
  u16 ind;
  s64 i;
  i = int_from_data(data, size);
  if (int_overflowed(i)) {
    *num = 0;
    return SN_ERR_OVERFLOWED_INT;
  }
  *num = num_pool_new(&ind);
  (*num)->val = i;
  (*num)->ind = ind;
  return SN_SUCCESS;
}


// Return a script num only if it's still a 4-byte integer
enum sn_result
sn_from_val(struct val val, struct num ** sn, int require_minimal) {

  switch (val.type) {
  case VT_SMALLINT:
  case VT_SCRIPTNUM:
    *sn = num_pool_get(val.ind);
    assert((*sn)->ind == val.ind);
    assert(*sn);

    // we're trying to return a scriptnum that is larger than 4 bytes. This is not
    // a valid scriptnum at this point. return an error.
    if (sn_overflowed(*sn)) {
      *sn = 0;
      return SN_ERR_OVERFLOWED_INT;
    }

    if (val.type == VT_SMALLINT)
      (*sn)->val = val.ind;

    return SN_SUCCESS;
  case VT_DATA: {
    u8 *data;
    u32 size;
    enum sn_result res;
    assert(val.ind != -1);
    data = byte_pool_get(val.ind, &size);
    return sn_from_data(data, size, sn);
  }
  }

  printf("type: %d\n", val.type);
  assert(!"sn_from_val: unhandled");

  return SN_SUCCESS;
}

struct val
sn_to_val(struct num *sn) {
  struct val val;
  struct num *snref;
  u16 ind;

  if (sn_overflowed(sn)) {
    u16 ind, len;
    static u8 tmp[8];
    sn_serialize(sn, tmp, 8, &len);
    u8 *data = byte_pool_new(len, &ind);
    memcpy(data, tmp, len);
    val.type = VT_DATA;
    val.ind = ind;
  }
  else
  {
    val.type = VT_SCRIPTNUM;
    if (sn->ind > -1)
      val.ind = sn->ind;
    else {
      snref = num_pool_new(&ind);
      snref->val = sn->val;
      val.ind = ind;
    }

  }
  return val;
}
