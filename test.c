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


static void
cmp_data(const u8 *a, const u8 *b, int alen, int blen, const char *msg) {
  int i = 0;
  cmp_ok(alen, "==", blen);

  if (memcmp(a, b, blen) != 0) {
    printf("#\n# Failed data cmp test\n# > ");
    for (i = 0; i < alen; ++i)
      printf("%02x ", a[i] & 0xff);

    printf("\n# > ");
    for (i = 0; i < blen; ++i)
      printf("%02x ", b[i] & 0xff);

    printf("\n#\n");
    fail("%s: data should match", msg);
  }
  else
    pass("%s: data should match", msg);
}
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
  u8 in_script[] = { OP_1 };

  script_eval(in_script, 1, stack);

  stack_push_val(expected, smallintval(1));
  ok_stacks_equal(stack, expected, "test_simple");
}

TEST(test_nip) {
  u8 in_script[] = { OP_1, OP_2, OP_NIP };

  script_eval(in_script, ARRAY_SIZE(in_script), stack);

  stack_push_val(expected, smallintval(2));
  ok_stacks_equal(stack, expected, "test_nip");
}

TEST(test_2dup_not_enough_input) {
  u8 in_script[] = { OP_1, OP_2DUP };

  int res = script_eval(in_script, ARRAY_SIZE(in_script), stack);
  ok(res == 0, "2dup fail on small stack");
}

TEST(big_int_serializes_ok) {
  int len;
  static u8 buf[12];
  static u8 expected_in[] = { 0x04, 0xff, 0xff, 0xff, 0x7f,
                              0x04, 0xff, 0xff, 0xff, 0x7f, OP_ADD };
  static u8 expected_out[] = { 0x05, 0xfe, 0xff, 0xff, 0xff, 0 };

  script_push_int(script, 2147483647);
  script_push_int(script, 2147483647);
  stack_push_op(script, OP_ADD);

  script_serialize(script, buf, ARRAY_SIZE(buf), &len);

  cmp_data(buf, expected_in, len, ARRAY_SIZE(expected_in),
           "big int input serializes ok");

  script_eval(buf, ARRAY_SIZE(expected_in), stack);
  script_serialize(stack, buf, ARRAY_SIZE(buf), &len);

  cmp_data(buf, expected_out, len, ARRAY_SIZE(expected_out),
           "big int output serializes ok");
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
run_test(struct stack *script, struct stack *stack, struct stack *expected, program *prog)
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

  g_silence_script_err = 0;
  g_silence_script_warn = 1;

  alloc_arenas();

  plan(5);

  stack_init(script);
  stack_init(stack);
  stack_init(expected);

  RUNTEST(test_simple);
  RUNTEST(test_nip);
  RUNTEST(test_2dup_not_enough_input);
  RUNTEST(big_int_serializes_ok);

  stack_free(script);
  stack_free(expected);
  stack_free(stack);

  free_arenas(0);

  done_testing();
  return 0;
}
