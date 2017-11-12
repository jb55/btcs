
#ifndef BTCS_ALLOC_H
#define BTCS_ALLOC_H

#include "consts.h"
#include "script_num.h"
#include "misc.h"

// MAX_OPS_PER_SCRIPT *
#define MAX_PUSHDATA_REFS (MAX_OPS_PER_SCRIPT * MAX_STACK_SIZE)
#define ALLOC_PUSHDATA_BYTES (MAX_STACK_SIZE * MAX_OPS_PER_SCRIPT)

struct num * num_pool_new(int *ind);
struct num * num_pool_pop();
struct num * num_pool_get(const int ind);

u8 *byte_pool_new(const u16 len, u16 *ind);
u8 *byte_pool_get(const int ind, u16 *len);
u8 *byte_pool_pop();

struct arenas {
  struct num *nums;
  u8 *bytes;
  u8 *bytes_top;
  struct stack bytes_map;
  int nbytes;
  int num_count;
};

void alloc_arena_sizes(struct arenas *, const int nums, const int bytes);
void alloc_arenas();
void free_arenas(struct arenas *arenas);

#endif /* BTCS_ALLOC_H */
