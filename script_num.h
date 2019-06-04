
#ifndef BTCS_SCRIPTNUM_H
#define BTCS_SCRIPTNUM_H

#include "misc.h"
#include "val.h"

enum sn_result {
  SN_SUCCESS,
  SN_ERR_OVERFLOWED_INT
};

struct num {
  s64 val;
  int ind;
};

void
sn_from_int(s64 n, struct num *);

int
sn_overflowed(struct num *num);

enum sn_result
sn_from_val(struct val val, struct num ** sn);

struct val
sn_to_val(struct num *sn);

int
sn_to_int(struct num *sn, int require_minimal);

void
sn_serialize(struct num *sn, u8 *buf, int bufsize, u16 *len);


inline static void sn_add(struct num *a, struct num *b, struct num *c) {
  c->val = a->val + b->val;
}

inline static void sn_sub(struct num *a, struct num *b, struct num *c) {
  c->val = a->val - b->val;
}

inline static void sn_neg(struct num *a, struct num *b) {
  b->val = -(a->val);
}


#endif /* BTCS_SCRIPTNUM_H */
