#pragma GCC diagnostic ignored "-Wint-conversion"

#include "script.h"
#include "op.h"
#include "tap.c/tap.h"

typedef void (program)(struct stack *script, struct stack *stack,\
                       struct stack *expected);

#define RUNTEST(test) run_test(script, stack, expected, test)
#define TEST(test) static inline void test(struct stack *script, \
                                           struct stack *stack, \
                                           struct stack *expected \
                                          )

/* static void */
/* test_nip(struct stack *stack, struct stack *expected) { */
/*   stack_push_op(OP_1); */
/* } */

static inline void
ok_stacks_equal(struct stack *s1, struct stack *s2, const char *context) {
  void **b1 = s1->bottom;
  void **b2 = s2->bottom;

  size_t s1size = stack_size(s1);
  size_t s2size = stack_size(s2);

  cmp_ok(s1size, "==", s2size, "%s: expected stack size", context);
  is(memcmp(b1, b2, s1size*sizeof(void*)), 0, "%s: expected stack memory", context);
}

TEST(test_simple) {
  stack_push_op(script, OP_1);

  script_eval(script, stack);

  struct val one = { .is_big = 0, .type = VT_INT, .val = 1 };
  stack_push_val(expected, one);
  ok_stacks_equal(stack, expected, "test_simple");
}

TEST(test_nip) {
  stack_push_op(script, OP_1);
  stack_push_op(script, OP_2);
  stack_push_op(script, OP_NIP);

  script_eval(script, stack);

  struct val two = { .is_big = 0, .type = VT_INT, .val = 2 };
  stack_push_val(expected, two);
  ok_stacks_equal(stack, expected, "test_nip");
}


static inline void
run_test(struct stack *script, struct stack *stack, struct stack *expected,
         program *prog)
{
  stack_clear(script);
  stack_clear(stack);
  stack_clear(expected);
  prog(script, stack, expected);
}


int
main(int argc, char *argv[]) {
  struct stack _stack, _expected, _script;
  struct stack *script   = &_script;
  struct stack *stack    = &_stack;
  struct stack *expected = &_expected;

  plan(4);

  stack_init(script);
  stack_init(stack);
  stack_init(expected);

  RUNTEST(test_simple);
  RUNTEST(test_nip);

  stack_free(expected);
  stack_free(stack);
  stack_free(script);

  done_testing();
  return 0;
}
