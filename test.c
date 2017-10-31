#pragma GCC diagnostic ignored "-Wint-conversion"

#include "script.h"
#include "op.h"
#include "alloc.h"
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
  struct val v1;
  struct val v2;

  size_t s1size = stack_size(s1);
  size_t s2size = stack_size(s2);

  cmp_ok(s1size, "==", s2size, "%s: expected stack size", context);

  for (size_t i = 0; i < s1size; ++i) {
    v1 = stack_top_val(s1, i);
    v2 = stack_top_val(s2, i);

    if (!val_eq(v1, v2, 0)) {
      is(val_name(v1), val_name(v2), "%s: stack vals match", context);
    }
  }
}

TEST(test_simple) {
  stack_push_op(script, OP_1);

  script_eval(script, stack);

  stack_push_val(expected, smallintval(1));
  ok_stacks_equal(stack, expected, "test_simple");
}

TEST(test_nip) {
  stack_push_op(script, OP_1);
  stack_push_op(script, OP_2);
  stack_push_op(script, OP_NIP);

  script_eval(script, stack);

  stack_push_val(expected, smallintval(2));
  ok_stacks_equal(stack, expected, "test_nip");
}

TEST(test_2dup_not_enough_input) {
  stack_push_op(script, OP_1);
  stack_push_op(script, OP_2DUP);

  int res = script_eval(script, stack);
  ok(res == 0, "2dup fail on small stack");
}


// TODO test scriptnum overflows
// TODO test scriptnum negative zero boolean logic
// TODO test scriptnum add into overflow + hash
// TODO test unpooled scriptnum index is == -1 for various constructors

/* TODO test ops:
 *
 *    OP_1ADD
 *    OP_1SUB
 *    OP_NEGATE
 *    OP_ABS
 *    OP_NOT
 *    OP_0NOTEQUAL
 *
 */


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

  alloc_arenas();

  plan(5);

  stack_init(script);
  stack_init(stack);
  stack_init(expected);

  RUNTEST(test_simple);
  RUNTEST(test_nip);
  RUNTEST(test_2dup_not_enough_input);

  stack_free(expected);
  stack_free(stack);
  stack_free(script);

  free_arenas(0);

  done_testing();
  return 0;
}
