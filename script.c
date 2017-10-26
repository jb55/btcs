
#include "script.h"
#include "op.h"
#include "stack.h"
#include <stdio.h>

void script_add_error(const char *serror) {
  // TODO: set_error
  fprintf(stderr, "error: %s\n", serror);
}

struct val
script_num(int n) {
  // TODO: scriptnum arithmetic
  struct val val;
  val.type = VT_INT;
  val.is_big = n > (2^COMPACT_VAL_BITS)-1;

  if (!val.is_big) {
    val.val = n;
    return val;
  }
  else {
    // looks like we have an int bigger than COMPACT_VAL_BITS in size,
    // we'll need to store it in a buffer somewhere
    assert(!"Implement script_num");
  }
}


int
cast_to_bool(struct val val) {
  // TODO: implement cast_to_bool
  assert(!"Implement cast_to_bool");
  /* for (unsigned int i = 0; i < vch.size(); i++) */
  /*   { */
  /*     if (vch[i] != 0) */
  /*       { */
  /*         // Can be negative zero */
  /*         if (i == vch.size()-1 && vch[i] == 0x80) */
  /*           return false; */
  /*         return true; */
  /*       } */
  /*   } */
  /* return false; */
}

void
script_eval(struct stack *script, struct stack *stack) {
  int op_count = 0;
  void **p = script->bottom;
  struct val val;
  struct stack _altstack;
  struct stack *altstack = &_altstack;
  int flags = 0;
  u8 tmpbuf[32];
  stack_init(altstack);

  while (p < script->top) {
    enum opcode opcode = *(enum opcode*)p;
    p++;
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
      struct val sn = script_num((int)opcode - (int)(OP_1 - 1));
      stack_push_val(stack, sn);
    }
    break;

    case OP_NOP: break;

    case OP_NOP1: case OP_NOP4: case OP_NOP5:
    case OP_NOP6: case OP_NOP7: case OP_NOP8: case OP_NOP9: case OP_NOP10:
      {
        /* if (script->flags & SCRIPT_VERIFY_DISCOURAGE_UPGRADABLE_NOPS) */
        script_add_warning("SCRIPT_ERR_DISCOURAGE_UPGRADABLE_NOPS");
      }
      break;

    /* case OP_IF: */
    /* case OP_NOTIF: */
    /* { */
    /*     // <expression> if [statements] [else [statements]] endif */
    /*     int fval = 0; */
    /*     if (fExec) */
    /*     { */
    /*         if (stack.size() < 1) */
    /*             return set_error(serror, SCRIPT_ERR_UNBALANCED_CONDITIONAL); */
    /*         valtype& vch = stacktop(-1); */
    /*         if (sigversion == SIGVERSION_WITNESS_V0 && (flags & SCRIPT_VERIFY_MINIMALIF)) { */
    /*             if (vch.size() > 1) */
    /*                 return set_error(serror, SCRIPT_ERR_MINIMALIF); */
    /*             if (vch.size() == 1 && vch[0] != 1) */
    /*                 return set_error(serror, SCRIPT_ERR_MINIMALIF); */
    /*         } */
    /*         fValue = CastToBool(vch); */
    /*         if (opcode == OP_NOTIF) */
    /*             fValue = !fValue; */
    /*         popstack(stack); */
    /*     } */
    /*     vfExec.push_back(fValue); */
    /* } */
    /* break; */


    case OP_INVALIDOPCODE:
    {
      return script_add_error("SCRIPT_ERR_INVALID_OPCODE");
    }
    break;

    case OP_FROMALTSTACK:
    {
        if (stack_size(altstack) < 1)
            return script_add_error("SCRIPT_ERR_INVALID_ALTSTACK_OPERATION");
        stack_push(stack, stack_top(altstack, -1));
        stack_pop(altstack);
    }
    break;

    case OP_2DROP:
    {
        // (x1 x2 -- )
        if (stack_size(stack) < 2)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        stack_pop(stack);
        stack_pop(stack);
    }
    break;

    case OP_2DUP:
    {
        // (x1 x2 -- x1 x2 x1 x2)
        if (stack_size(stack) < 2)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val v1 = stack_top_val(stack, -2);
        struct val v2 = stack_top_val(stack, -1);
        stack_push_val(stack, v1);
        stack_push_val(stack, v2);
    }
    break;

    case OP_3DUP:
    {
        // (x1 x2 x3 -- x1 x2 x3 x1 x2 x3)
        if (stack_size(stack) < 3)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val v1 = stack_top_val(stack, -3);
        struct val v2 = stack_top_val(stack, -2);
        struct val v3 = stack_top_val(stack, -1);
        stack_push_val(stack, v1);
        stack_push_val(stack, v2);
        stack_push_val(stack, v3);
    }
    break;

    case OP_2OVER:
    {
        // (x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2)
        if (stack_size(stack) < 4)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val v1 = stack_top_val(stack, -4);
        struct val v2 = stack_top_val(stack, -3);
        stack_push_val(stack, v1);
        stack_push_val(stack, v2);
    }
    break;

    case OP_2ROT:
    {
        // (x1 x2 x3 x4 x5 x6 -- x3 x4 x5 x6 x1 x2)
        if (stack_size(stack) < 6)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val t6 = stack_top_val(stack, -6);
        struct val t5 = stack_top_val(stack, -5);
        *(stack->top - 6) = *(stack->top - 4);
        *(stack->top - 5) = *(stack->top - 3);
        *(stack->top - 4) = *(stack->top - 2);
        *(stack->top - 3) = *(stack->top - 1);
        stack_set_val(stack, -2, t6);
        stack_set_val(stack, -1, t5);
    }
    break;

    case OP_2SWAP:
    {
        // (x1 x2 x3 x4 -- x3 x4 x1 x2)
        if (stack_size(stack) < 4)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");

        struct val t4 = stack_top_val(stack, -4);
        struct val t3 = stack_top_val(stack, -3);

        *(stack->top - 4) = *(stack->top - 2);
        *(stack->top - 3) = *(stack->top - 1);
        stack_set_val(stack, -2, t4);
        stack_set_val(stack, -1, t3);
    }
    break;

    case OP_IFDUP:
    {
        // (x - 0 | x x)
        if (stack_size(stack) < 1)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, -1);
        if (cast_to_bool(val))
            stack_push_val(stack, val);
    }
    break;

    case OP_DEPTH:
    {
        // -- stacksize
      struct val sn = script_num(stack_size(stack));
      stack_push_val(stack, sn);
    }
    break;

    case OP_DROP:
    {
      // (x -- )
      if (stack_size(stack) < 1)
          return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
      stack_pop(stack);
    }
    break;

    case OP_DUP:
    {
        // (x -- x x)
        if (stack_size(stack) < 1)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, (-1));
        stack_push_val(stack, val);
    }
    break;

    case OP_NIP:
    {
        // (x1 x2 -- x2)
        if (stack_size(stack) < 2)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        // TODO: sanity check - stack_size() == stack_end(stack)
        stack_set_val(stack, -2, stack_top_val(stack, -1));
        stack_pop(stack);
    }
    break;

    case OP_OVER:
    {
        // (x1 x2 -- x1 x2 x1)
        if (stack_size(stack) < 2)
            return script_add_error("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, (-2));
        stack_push_val(stack, val);
    }
    break;

    default: {
      sprintf((char*)tmpbuf, "unhandled opcode %s", op_name(opcode));
      return script_add_error((char*)tmpbuf);
    }

    }
  }

}

void script_print_ops(struct stack *stack) {
  void **p = stack->bottom;
  while (p < stack->top) {
    enum opcode opcode = (enum opcode)*p++;
    printf("%s ", op_name(opcode));
  }
  printf("\n");
}

void script_print_vals(struct stack *stack) {
  void **p = stack->bottom;
  while (p < stack->top) {
    struct val val;
    memcpy(&val, &*p++, sizeof(struct val));
    val_print(val);
    putchar(' ');
  }
  putchar('\n');
}


int script_init(struct script *script) {
  stack_init(&script->pushdata);
  return stack_init(&script->data);
}

void script_free(struct script *script) {
  stack_free(&script->data);
  void **p = script->pushdata.bottom;
  assert(p);
  while (p < script->pushdata.top) {
    assert(*p);
    p++;
  }
}

int script_new(struct script *script) {
  return 0;
}
