
#include "script_num.h"
#include "alloc.h"
#include "val.h"

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
sn_from_int(int n, struct num *sn) {
  sn->ind = -1;
  sn->val = n;
}


/* enum sn_result */
/* sn_to_int(struct num *sn, int *res) { */
/*   if (sn_overflowed(sn)) { */
/*     return SN_ERR_OVERFLOWED_INT; */
/*   } */
/* } */


const char *
sn_serialize(struct num *sn, u16 *len) {
  assert(!"implement sn_serialize");
  return "";
}


int
sn_overflowed(struct num *num) {
  return (num->val & ~0xFFFFFFFF) != 0;
}


// Return a script num only if it's still a 4-byte integer
enum sn_result
sn_from_val(struct val val, struct num ** sn, int require_minimal) {
  switch (val.type) {
  case VT_SCRIPTNUM:
    *sn = num_pool_get(val.ind);
    assert(*sn);

    // we're trying to return a scriptnum that is larger than 4 bytes. This is not
    // a valid scriptnum at this point. return an error.
    if (sn_overflowed(*sn)) {
      *sn = 0;
      return SN_ERR_OVERFLOWED_INT;
    }

    break;
  case VT_DATA:
    assert(!"TODO implement raw data to script num");
  }

  return SN_SUCCESS;
}

struct val
sn_to_val(struct num *sn) {
  struct val val;
  struct num *snref;
  int ind;

  // TODO: implement return overflowed scriptnums
  assert(!sn_overflowed(sn));

  val.type = VT_SCRIPTNUM;
  if (sn->ind > -1)
    val.ind = sn->ind;
  else {
    snref = num_pool_new(&ind);
    *sn = *snref;
    val.ind = ind;
  }
  return val;
}
