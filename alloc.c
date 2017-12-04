

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "alloc.h"
#include "stack.h"

static struct arenas g_arenas;

void
alloc_arenas() {
  alloc_arena_sizes(0, MAX_STACK_SIZE, MAX_STACK_SIZE * MAX_STACK_SIZE);
}

void
alloc_arena_sizes(struct arenas *arenas, const int nums, const int bytes) {
  if (!arenas) arenas = &g_arenas;
  arenas->nums = (struct num*)calloc(nums, sizeof(struct num));
  assert(arenas->nums);
  arenas->bytes = (u8*)calloc(bytes, 1);
  stack_init_size(&arenas->bytes_map, bytes);
  arenas->bytes_top = arenas->bytes;
  arenas->num_count = 0;
  arenas->nbytes = bytes;
}

struct num *
num_pool_get(const int ind) {
  return &g_arenas.nums[ind];
}


struct num *
num_pool_pop() {
  assert(g_arenas.num_count > 0);
  return &g_arenas.nums[g_arenas.num_count--];
}


struct num *
num_pool_new(int *ind) {
  *ind = g_arenas.num_count++;
  struct num *p;
  p = &g_arenas.nums[*ind];
  assert(p);
  assert(p->val == 0 && p->ind == 0);
  p->ind = *ind;
  return p;
}

u8 *
byte_pool_new(u32 len, u16 *ind) {
  assert((g_arenas.bytes_top - g_arenas.bytes + len) <= g_arenas.nbytes);
  u8 *start = g_arenas.bytes_top;
  u32 *c = (u32*)g_arenas.bytes_top;
  *c++ = len;
  u8 *p = (u8*)c;
  p += len;
  *ind = stack_size(&g_arenas.bytes_map);
  stack_push(&g_arenas.bytes_map, (void*)start);
  g_arenas.bytes_top = p;
  assert(*p == 0);
  assert(((g_arenas.bytes_top - g_arenas.bytes) + len) <= g_arenas.nbytes);
  return p - len;
}


// useful for quick alloc/deallocs
u8 *
byte_pool_pop() {
  u8 *p = (u8*)stack_pop(&g_arenas.bytes_map);
  u32 *c = (u32*)p;
  memset(p, 0, *c + sizeof(u32));
  return p;
}


u8 *
byte_pool_get(int ind, u32 *len) {
  assert(ind <= stack_size(&g_arenas.bytes_map) - 1);
  void **vp;
  u8 *p;
  u32 *up;
  vp = g_arenas.bytes_map.bottom + ind;
  p = (u8*)(*vp);
  assert((g_arenas.bytes_top - g_arenas.bytes + *len) <= g_arenas.nbytes);
  assert(p);
  up = (u32*)p;
  *len = *up++;
  p = (u8*)up;
  assert((g_arenas.bytes_top - g_arenas.bytes + *len) <= g_arenas.nbytes);
  return p;
}

void
free_arenas(struct arenas *arenas) {
  if (!arenas) arenas = &g_arenas;
  if (!arenas) return;
  stack_free(&arenas->bytes_map);
  free(arenas->bytes);
  free(arenas->nums);
}

