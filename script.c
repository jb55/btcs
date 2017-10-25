
#include "script.h"
#include "op.h"
#include "stack.h"
#include <stdio.h>

void script_add_error(const char *serror) {
  // TODO: set_error
  printf("error: %s\n", serror);
}

u8
script_num(int n) {
  // TODO: scriptnum
  /* assert(!"Implement script_num"); */
  return (u8)n;
}


void
script_eval(struct stack *script, struct stack *stack) {
  int op_count = 0;
  u8 *p = script->bottom;
  struct stack tmpstack;

  while (p < script->top) {
    u8 opcode = *p++;
    // TODO: pushdata ops
    assert(!(opcode >= OP_PUSHDATA1 && opcode <= OP_PUSHDATA4));

    if (opcode > OP_16 && ++op_count > MAX_OPS_PER_SCRIPT)
      script_add_error("MAX_OPS_PER_SCRIPT");

    if (opcode == OP_CAT ||
        opcode == OP_SUBSTR ||
        opcode == OP_LEFT ||
        opcode == OP_RIGHT ||
        opcode == OP_INVERT ||
        opcode == OP_AND ||
        opcode == OP_OR ||
        opcode == OP_XOR ||
        opcode == OP_2MUL ||
        opcode == OP_2DIV ||
        opcode == OP_MUL ||
        opcode == OP_DIV ||
        opcode == OP_MOD ||
        opcode == OP_LSHIFT ||
        opcode == OP_RSHIFT) {

      script_add_error("SCRIPT_ERR_DISABLED_OPCODE"); // Disabled opcodes.
    }


    switch (opcode) {
    case OP_1NEGATE:
    case OP_1:
    case OP_2:
    case OP_3:
    case OP_4:
    case OP_5:
    case OP_6:
    case OP_7:
    case OP_8:
    case OP_9:
    case OP_10:
    case OP_11:
    case OP_12:
    case OP_13:
    case OP_14:
    case OP_15:
    case OP_16:
    {
      u8 sn = script_num((int)opcode - (int)(OP_1 - 1));
      stack_push(stack, sn);
    }
    break;

    case OP_NOP: break;

    case OP_2DUP:
    {
      // (x1 x2 -- x1 x2 x1 x2)
      if (stack_size(stack) < 2)
        return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
      u8 v1 = stack_top(stack, -2);
      u8 v2 = stack_top(stack, -1);
      stack_push(stack, v1);
      stack_push(stack, v2);
    }
    break;
    }
  }

}

void script_print(struct stack *stack) {
  u8 *p = stack->bottom;
  while (p < stack->top)
    printf("%s ", op_name(*p++));
  printf("\n");
}


int script_init(struct script *script) {
  return stack_init(&script->data);
}

void script_free(struct script *script) {
  stack_free(&script->data);
}
