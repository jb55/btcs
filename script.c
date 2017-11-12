
#include "script.h"
#include "op.h"
#include "script_num.h"
#include "stack.h"
#include "alloc.h"
#include "valstack.h"
#include <stdio.h>

#define SCRIPTERR(serr) script_add_error(c, opcode, serr)

int g_silence_script_err = 0;
int g_silence_script_warn = 0;

int script_add_error(int c, enum opcode cur_op, const char *serror) {
  // TODO: set_error
  if (!g_silence_script_err)
    fprintf(stderr, "error: %s @ op %d (%s)\n", serror, c, op_name(cur_op));
  return 0;
}

void script_add_warning(const char *warning) {
  // TODO: set_error
  if (!g_silence_script_warn)
    fprintf(stderr, "warning: %s\n", warning);
}


int
cast_to_bool(struct val val) {
  // TODO: implement cast_to_bool
  switch (val.type) {
  case VT_SCRIPTNUM: {
    struct num *sn = num_pool_get(val.ind);
    return sn->val != 0;
  }
  case VT_DATA: {
    u16 len;
    const u8 * bytes = byte_pool_get(val.ind, &len);
    return *bytes != 0;
  }
  }
  assert(!"Unhandled val.type in cast_to_bool");
}

int
script_getop(u8 **p, u8 *end, enum opcode *popcode, u8 *buf, int bufsize, u32 *outlen) {
  *popcode = OP_INVALIDOPCODE;
  u32 nsize = 0;

  int opcode;

  if (buf)
    memset(buf, 0, bufsize);

  opcode = **p;
  *p = *p + 1;

  if (opcode <= OP_PUSHDATA4) {
    if (opcode < OP_PUSHDATA1) {
      nsize = opcode;
    }
    else {
      switch (opcode) {
      case OP_PUSHDATA1:
        if ((end - *p) < 1) {
          return 0;
        }
        nsize = *(*p++);
        break;
      case OP_PUSHDATA2:
        if ((end - *p) < 2) {
          return 0;
        }
        nsize = readle16(*p);
        *p += 2;
        break;
      case OP_PUSHDATA4:
        if ((end - *p) < 4) {
          return 0;
        }
        nsize = readle32(*p);
        *p += 4;
        break;
      default:
        break;
      }
    }

    if ((end - *p) < 0 || (u32)(end - *p) < nsize) {
      return 0;
    }

    if (buf) {
      *outlen = nsize;
      memcpy(buf, *p, nsize);
    }
    *p += nsize;
  }

  *popcode = opcode;

  return 1;
}


int
script_eval(u8 *script, size_t script_size, struct stack *stack) {
  int op_count = 0;
  u32 tmplen;
  u8 *p = script;
  u8 *top = script + script_size;
  static char tmpbuf[32];
  static const struct val val_true  = {.type = VT_SMALLINT, .ind  = 1};
  static const struct val val_false = {.type = VT_SMALLINT, .ind  = 0};
  static const struct num bn_one  = {.val = 1, .ind = -1};
  static const struct num bn_zero = {.val = 0, .ind = -1};
  struct val val;
  struct stack _altstack;
  struct stack _ifstack;
  struct stack *altstack = &_altstack;
  struct stack *ifstack = &_ifstack;
  int flags = CO_WARNINGS_ARE_ERRORS | CO_WARN_MINIMAL;
  int c = 0;
  // TODO: require minimal?
  int require_minimal =
    !(flags & ~(CO_WARNINGS_ARE_ERRORS | CO_WARN_MINIMAL));
  stack_init(altstack);
  stack_init(ifstack);

  while (p < top) {
    c++;
    enum opcode opcode;
    script_getop(&p, top, &opcode, (u8*)tmpbuf, ARRAY_SIZE(tmpbuf), &tmplen);
    int if_exec = !stack_size(ifstack);
    // Note OP_RESERVED does not count towards the opcode limit.
    if (opcode > OP_16 && ++op_count > MAX_OPS_PER_SCRIPT)
      SCRIPTERR("MAX_OPS_PER_SCRIPT");

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

      SCRIPTERR("SCRIPT_ERR_DISABLED_OPCODE"); // Disabled opcodes.
    }

    if (if_exec && 0 <= opcode && opcode <= OP_PUSHDATA4) {
      /* if (fRequireMinimal && !CheckMinimalPush(vchPushValue, opcode)) { */
      /*   return set_error(serror, SCRIPT_ERR_MINIMALDATA); */
      /* } */
      printf("pushing data of size %d\n", tmplen);
      stack_push_data(stack, (u8*)tmpbuf, tmplen);
    } else if (if_exec || (OP_IF <= opcode && opcode <= OP_ENDIF))
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
      struct num sn;
      sn_from_int((int)opcode - (int)(OP_1 - 1), &sn);
      struct val val = sn_to_val(&sn);
      stack_push_val(stack, val);
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

    case OP_IF:
    case OP_NOTIF:
    {
        // <expression> if [statements] [else [statements]] endif
        int ifval = 0;
        if (if_exec)
        {
            if (stack_size(stack) < 1)
              return SCRIPTERR("SCRIPT_ERR_UNBALANCED_CONDITIONAL");
            struct val val = stack_top_val(stack, -1);
            /* if (sigversion == SIGVERSION_WITNESS_V0 && (flags & SCRIPT_VERIFY_MINIMALIF)) { */
            /*     if (vch.size() > 1) */
            /*         return SCRIPTERR(SCRIPT_ERR_MINIMALIF); */
            /*     if (vch.size() == 1 && vch[0] != 1) */
            /*         return SCRIPTERR(SCRIPT_ERR_MINIMALIF); */
            /* } */
            ifval = cast_to_bool(val);
            if (opcode == OP_NOTIF)
                ifval = !ifval;
            stack_pop(stack);
        }
        stack_push_small(int, ifstack, &ifval);
    }
    break;


    case OP_INVALIDOPCODE:
    {
      return SCRIPTERR("SCRIPT_ERR_INVALID_OPCODE");
    }
    break;

    case OP_FROMALTSTACK:
    {
        if (stack_size(altstack) < 1)
          return SCRIPTERR("SCRIPT_ERR_INVALID_ALTSTACK_OPERATION");
        stack_push(stack, stack_top(altstack, -1));
        stack_pop(altstack);
    }
    break;

    case OP_2DROP:
    {
        // (x1 x2 -- )
        if (stack_size(stack) < 2)
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        stack_pop(stack);
        stack_pop(stack);
    }
    break;

    case OP_2DUP:
    {
        // (x1 x2 -- x1 x2 x1 x2)
        if (stack_size(stack) < 2)
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
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
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
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
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
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
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
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
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");

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
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, -1);
        if (cast_to_bool(val))
            stack_push_val(stack, val);
    }
    break;

    case OP_DEPTH:
    {
        // -- stacksize
      struct num sn;
      sn_from_int(stack_size(stack), &sn);
      struct val val = sn_to_val(&sn);
      stack_push_val(stack, val);
    }
    break;

    case OP_DROP:
    {
      // (x -- )
      if (stack_size(stack) < 1)
        return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
      stack_pop(stack);
    }
    break;

    case OP_DUP:
    {
        // (x -- x x)
        if (stack_size(stack) < 1)
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, (-1));
        stack_push_val(stack, val);
    }
    break;

    case OP_NIP:
    {
        // (x1 x2 -- x2)
        if (stack_size(stack) < 2)
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        // TODO: sanity check - stack_size() == stack_end(stack)
        stack_set_val(stack, -2, stack_top_val(stack, -1));
        stack_pop(stack);
    }
    break;

    case OP_OVER:
    {
        // (x1 x2 -- x1 x2 x1)
        if (stack_size(stack) < 2)
          return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, (-2));
        stack_push_val(stack, val);
    }
    break;


    case OP_PICK:
    case OP_ROLL:
    {
        // (xn ... x2 x1 x0 n - xn ... x2 x1 x0 xn)
        // (xn ... x2 x1 x0 n - ... x2 x1 x0 xn)
        if (stack_size(stack) < 2)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct num *n;

        enum sn_result res =
          sn_from_val(stack_top_val(stack, -1), &n, require_minimal);

        if (res != SN_SUCCESS) {
          sprintf(tmpbuf, "invalid scriptnum %d", res);
          SCRIPTERR(tmpbuf);
        }

        stack_pop(stack);
        if (n->val < 0 || n->val >= (int)stack_size(stack))
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, (-(n->val))-1);
        if (opcode == OP_ROLL)
          assert(!"Finish OP_ROLL");
            /* stack.erase(stack.end()-n-1); */
        stack_push_val(stack, val);
    }
    break;

    case OP_ROT:
    {
        // (x1 x2 x3 -- x2 x3 x1)
        //  x2 x1 x3  after first swap
        //  x2 x3 x1  after second swap
        if (stack_size(stack) < 3)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        stack_swap(stack, -3, -2);
        stack_swap(stack, -2, -1);
    }
    break;

    case OP_SWAP:
    {
      // (x1 x2 -- x2 x1)
      if (stack_size(stack) < 2)
        return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
      stack_swap(stack, -2, -1);
    }
    break;

    case OP_TUCK:
    {
        // (x1 x2 -- x2 x1 x2)
        if (stack_size(stack) < 2)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val val = stack_top_val(stack, -1);
        stack_swap(stack, -2, -1);
        stack_push(stack, stack_top(stack, -2));
    }
    break;


    case OP_SIZE:
    {
        // (in -- in size)
        if (stack_size(stack) < 1)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct num sn;
        sn_from_int(stack_size(stack) - 1, &sn);
        struct val val = sn_to_val(&sn);
        stack_push_val(stack, val);
    }
    break;


    //
    // Bitwise logic
    //
    case OP_EQUAL:
    case OP_EQUALVERIFY:
    //case OP_NOTEQUAL: // use OP_NUMNOTEQUAL
    {
        // (x1 x2 - bool)
        if (stack_size(stack) < 2)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct val v1 = stack_top_val(stack, -2);
        struct val v2 = stack_top_val(stack, -1);
        int equal = val_eq(v1, v2, require_minimal);
        // OP_NOTEQUAL is disabled because it would be too easy to say
        // something like n != 1 and have some wiseguy pass in 1 with extra
        // zero bytes after it (numerically, 0x01 == 0x0001 == 0x000001)
        //if (opcode == OP_NOTEQUAL)
        //    fEqual = !fEqual;
        stack_pop(stack);
        stack_pop(stack);
        stack_push_val(stack, equal ? val_true : val_false);
        if (opcode == OP_EQUALVERIFY)
        {
            if (equal)
                stack_pop(stack);
            else
                return SCRIPTERR("SCRIPT_ERR_EQUALVERIFY");
        }
    }
    break;


    //
    // Numeric
    //
    case OP_1ADD:
    case OP_1SUB:
    case OP_NEGATE:
    case OP_ABS:
    case OP_NOT:
    case OP_0NOTEQUAL:
    {
        // (in -- out)
        if (stack_size(stack) < 1)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct num *bn;
        enum sn_result res =
          sn_from_val(stack_top_val(stack, -1), &bn, require_minimal);

        if (res != SN_SUCCESS) {
          sprintf(tmpbuf, "invalid scriptnum %d", res);
          SCRIPTERR(tmpbuf);
        }

        switch (opcode)
        {
        case OP_1ADD:       bn->val += 1; break;
        case OP_1SUB:       bn->val -= 1; break;
        case OP_NEGATE:     bn->val = -bn->val; break;
        case OP_ABS:
          if (bn->val < bn_zero.val)
            bn->val = -(bn->val);
          break;
        case OP_NOT:        bn->val = (bn->val == 0); break;
        case OP_0NOTEQUAL:  bn->val = (bn->val != 0); break;
        default:            assert(!"invalid opcode"); break;
        }
        stack_pop(stack);
        stack_push_val(stack, sn_to_val(bn));
    }
    break;

    case OP_ADD:
    case OP_SUB:
    case OP_BOOLAND:
    case OP_BOOLOR:
    case OP_NUMEQUAL:
    case OP_NUMEQUALVERIFY:
    case OP_NUMNOTEQUAL:
    case OP_LESSTHAN:
    case OP_GREATERTHAN:
    case OP_LESSTHANOREQUAL:
    case OP_GREATERTHANOREQUAL:
    case OP_MIN:
    case OP_MAX:
    {
        // (x1 x2 -- out)
        if (stack_size(stack) < 2)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct num *bn1, *bn2, bn;
        sn_from_val(stack_top_val(stack, -2), &bn1, require_minimal);
        sn_from_val(stack_top_val(stack, -1), &bn2, require_minimal);
        /* struct num bn(0); */
        switch (opcode)
        {
        case OP_ADD:
            bn.val = bn1->val + bn2->val;
            break;

        case OP_SUB:
            bn.val = bn1->val - bn2->val;
            break;

        case OP_BOOLAND:             bn.val = bn1->val != 0 && bn2->val != 0;
          break;
        case OP_BOOLOR:              bn.val = bn1->val != 0 || bn2->val != 0;
          break;
        case OP_NUMEQUAL:            bn.val = bn1->val == bn2->val; break;
        case OP_NUMEQUALVERIFY:      bn.val = bn1->val == bn2->val; break;
        case OP_NUMNOTEQUAL:         bn.val = bn1->val != bn2->val; break;
        case OP_LESSTHAN:            bn.val = bn1->val < bn2->val; break;
        case OP_GREATERTHAN:         bn.val = bn1->val > bn2->val; break;
        case OP_LESSTHANOREQUAL:     bn.val = bn1->val <= bn2->val; break;
        case OP_GREATERTHANOREQUAL:  bn.val = bn1->val >= bn2->val; break;
        case OP_MIN:
          bn.val = bn1->val < bn2->val ? bn1->val : bn2->val;
          break;
        case OP_MAX:
          bn.val = bn1->val > bn2->val ? bn1->val : bn2->val;
          break;
        default:                     assert(!"invalid opcode"); break;
        }
        stack_pop(stack);
        stack_pop(stack);
        stack_push_val(stack, sn_to_val(&bn));

        if (opcode == OP_NUMEQUALVERIFY)
        {
            if (cast_to_bool(stack_top_val(stack, -1)))
                stack_pop(stack);
            else
                return SCRIPTERR("SCRIPT_ERR_NUMEQUALVERIFY");
        }
    }
    break;

    case OP_WITHIN:
    {
        // (x min max -- out)
        if (stack_size(stack) < 3)
            return SCRIPTERR("SCRIPT_ERR_INVALID_STACK_OPERATION");
        struct num *bn1, *bn2, *bn3;
        sn_from_val(stack_top_val(stack, -3), &bn1, require_minimal);
        sn_from_val(stack_top_val(stack, -2), &bn2, require_minimal);
        sn_from_val(stack_top_val(stack, -1), &bn3, require_minimal);
        int fval = bn2->val <= bn1->val && bn1->val < bn3->val;
        stack_pop(stack);
        stack_pop(stack);
        stack_pop(stack);
        stack_push_val(stack, fval ? val_true : val_false);
    }
    break;

    default: {
      return SCRIPTERR("unhandled opcode");
    }

    }
  }

  stack_free(altstack);
  stack_free(ifstack);
  return 1;
}

void script_print(u8 *script, size_t script_size) {
  u32 len;
  static u8 tmpbuf[4096];
  u8 *t = tmpbuf;
  u8 *p = script;
  u8 *top = script + script_size;
  while (p < top) {
    enum opcode opcode;
    script_getop(&p, top, &opcode, tmpbuf, ARRAY_SIZE(tmpbuf), &len);
    u8 *ttop = tmpbuf + len;
    printf("%s ", op_name(opcode));
    if (t < ttop)
      printf("%02x", *t++);
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

void
script_push_int(struct stack *script, s64 intval) {
  /* u16 len; */
  /* u16 i; */
  /* static u8 buf[8]; */
  struct val val = val_from_int(intval);
  stack_push_val(script, val);
  /* val_serialize(val, &len, buf, ARRAY_SIZE(buf)); */
  /* for (i = 0; i < len; ++i) */
  /*   stack_push_small(u8, script, &buf[i]); */
}


void
script_serialize(struct stack *stack, u8 *buf, int buflen, int* len) {
  struct val *valp;
  void **sp;
  u8 *p = buf;
  u16 valsize;
  *len = 0;
  sp = stack->bottom;

  while (sp < stack->top) {
    /* printf("%02x %02x %02x %02x | ", huh[0], huh[1], huh[2], huh[3]); */
    /* printf("%d %d\n", val.type, val.ind); */
    valp = (struct val*)sp;
    val_print(*valp);
    printf("\n");
    val_serialize(*valp, &valsize, p, buflen-(p-buf));
    p += valsize;
    *len += valsize;
    assert(p-buf <= buflen);
    sp++;
  }
}
