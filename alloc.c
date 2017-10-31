

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
  arenas->bytes = (char*)calloc(bytes, 1);
  stack_init_size(&arenas->bytes_map, bytes);
  arenas->bytes_top = arenas->bytes;
  arenas->nbytes = bytes;
}

struct num *
num_pool_get(const int ind) {
  return &g_arenas.nums[ind];
}


struct num *
num_pool_new(int *ind) {
  *ind = g_arenas.num_count++;
  struct num *p;
  p = &g_arenas.nums[*ind];
  assert(p);
  assert(p->val == 0 && p->ind == 0);
  return p;
}

char *
byte_pool_new(const char *bytes, const u16 len) {
  assert(g_arenas.bytes_top - g_arenas.bytes + len <= g_arenas.nbytes);
  char *start = g_arenas.bytes_top;
  u16 *c = (u16*)g_arenas.bytes_top;
  *c++ = len;
  char *p = (char*)c;
  memcpy(p, bytes, len);
  p += len;
  g_arenas.bytes_top = p;
  assert(*p == 0);
  return start;
}


// useful for quick alloc/deallocs
char *
byte_pool_pop() {
  char *p = (char*)stack_pop(&g_arenas.bytes_map);
  u16 *c = (u16*)p;
  memset(p, 0, *c + sizeof(u16));
  return p;
}


char *
byte_pool_get(const int ind, u16 *len) {
  char *p;
  u16 *up;
  p = (char*)(g_arenas.bytes_map.bottom + ind);
  assert(p);
  up = (u16*)p;
  *len = *(up++);
  p = (char*)up;
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
