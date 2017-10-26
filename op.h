
#ifndef BCS_OP_H
#define BCS_OP_H

#include "misc.h"
#include "stack.h"
#include <stdio.h>
#include <math.h>

enum opcode
{
    // push value
    OP_0 = 0x00,
    OP_FALSE = OP_0,
    OP_PUSHDATA1 = 0x4c,
    OP_PUSHDATA2 = 0x4d,
    OP_PUSHDATA4 = 0x4e,
    OP_1NEGATE = 0x4f,
    OP_RESERVED = 0x50,
    OP_1 = 0x51,
    OP_TRUE=OP_1,
    OP_2 = 0x52,
    OP_3 = 0x53,
    OP_4 = 0x54,
    OP_5 = 0x55,
    OP_6 = 0x56,
    OP_7 = 0x57,
    OP_8 = 0x58,
    OP_9 = 0x59,
    OP_10 = 0x5a,
    OP_11 = 0x5b,
    OP_12 = 0x5c,
    OP_13 = 0x5d,
    OP_14 = 0x5e,
    OP_15 = 0x5f,
    OP_16 = 0x60,

    // control
    OP_NOP = 0x61,
    OP_VER = 0x62,
    OP_IF = 0x63,
    OP_NOTIF = 0x64,
    OP_VERIF = 0x65,
    OP_VERNOTIF = 0x66,
    OP_ELSE = 0x67,
    OP_ENDIF = 0x68,
    OP_VERIFY = 0x69,
    OP_RETURN = 0x6a,

    // stack ops
    OP_TOALTSTACK = 0x6b,
    OP_FROMALTSTACK = 0x6c,
    OP_2DROP = 0x6d,
    OP_2DUP = 0x6e,
    OP_3DUP = 0x6f,
    OP_2OVER = 0x70,
    OP_2ROT = 0x71,
    OP_2SWAP = 0x72,
    OP_IFDUP = 0x73,
    OP_DEPTH = 0x74,
    OP_DROP = 0x75,
    OP_DUP = 0x76,
    OP_NIP = 0x77,
    OP_OVER = 0x78,
    OP_PICK = 0x79,
    OP_ROLL = 0x7a,
    OP_ROT = 0x7b,
    OP_SWAP = 0x7c,
    OP_TUCK = 0x7d,

    // splice ops
    OP_CAT = 0x7e,
    OP_SUBSTR = 0x7f,
    OP_LEFT = 0x80,
    OP_RIGHT = 0x81,
    OP_SIZE = 0x82,

    // bit logic
    OP_INVERT = 0x83,
    OP_AND = 0x84,
    OP_OR = 0x85,
    OP_XOR = 0x86,
    OP_EQUAL = 0x87,
    OP_EQUALVERIFY = 0x88,
    OP_RESERVED1 = 0x89,
    OP_RESERVED2 = 0x8a,

    // numeric
    OP_1ADD = 0x8b,
    OP_1SUB = 0x8c,
    OP_2MUL = 0x8d,
    OP_2DIV = 0x8e,
    OP_NEGATE = 0x8f,
    OP_ABS = 0x90,
    OP_NOT = 0x91,
    OP_0NOTEQUAL = 0x92,

    OP_ADD = 0x93,
    OP_SUB = 0x94,
    OP_MUL = 0x95,
    OP_DIV = 0x96,
    OP_MOD = 0x97,
    OP_LSHIFT = 0x98,
    OP_RSHIFT = 0x99,

    OP_BOOLAND = 0x9a,
    OP_BOOLOR = 0x9b,
    OP_NUMEQUAL = 0x9c,
    OP_NUMEQUALVERIFY = 0x9d,
    OP_NUMNOTEQUAL = 0x9e,
    OP_LESSTHAN = 0x9f,
    OP_GREATERTHAN = 0xa0,
    OP_LESSTHANOREQUAL = 0xa1,
    OP_GREATERTHANOREQUAL = 0xa2,
    OP_MIN = 0xa3,
    OP_MAX = 0xa4,

    OP_WITHIN = 0xa5,

    // crypto
    OP_RIPEMD160 = 0xa6,
    OP_SHA1 = 0xa7,
    OP_SHA256 = 0xa8,
    OP_HASH160 = 0xa9,
    OP_HASH256 = 0xaa,
    OP_CODESEPARATOR = 0xab,
    OP_CHECKSIG = 0xac,
    OP_CHECKSIGVERIFY = 0xad,
    OP_CHECKMULTISIG = 0xae,
    OP_CHECKMULTISIGVERIFY = 0xaf,

    // expansion
    OP_NOP1 = 0xb0,
    OP_CHECKLOCKTIMEVERIFY = 0xb1,
    OP_NOP2 = OP_CHECKLOCKTIMEVERIFY,
    OP_CHECKSEQUENCEVERIFY = 0xb2,
    OP_NOP3 = OP_CHECKSEQUENCEVERIFY,
    OP_NOP4 = 0xb3,
    OP_NOP5 = 0xb4,
    OP_NOP6 = 0xb5,
    OP_NOP7 = 0xb6,
    OP_NOP8 = 0xb7,
    OP_NOP9 = 0xb8,
    OP_NOP10 = 0xb9,


    // template matching params
    OP_SMALLINTEGER = 0xfa,
    OP_PUBKEYS = 0xfb,
    OP_PUBKEYHASH = 0xfd,
    OP_PUBKEY = 0xfe,

    OP_INVALIDOPCODE = 0xff,
};


enum opcode_token
{
    // push value
    _OP_0=1,
    _OP_FALSE,
    _OP_PUSHDATA1,
    _OP_PUSHDATA2,
    _OP_PUSHDATA4,
    _OP_1NEGATE,
    _OP_RESERVED,
    _OP_1,
    _OP_TRUE,
    _OP_2,
    _OP_3,
    _OP_4,
    _OP_5,
    _OP_6,
    _OP_7,
    _OP_8,
    _OP_9,
    _OP_10,
    _OP_11,
    _OP_12,
    _OP_13,
    _OP_14,
    _OP_15,
    _OP_16,

    // control
    _OP_NOP,
    _OP_VER,
    _OP_IF,
    _OP_NOTIF,
    _OP_VERIF,
    _OP_VERNOTIF,
    _OP_ELSE,
    _OP_ENDIF,
    _OP_VERIFY,
    _OP_RETURN,

    // stack ops
    _OP_TOALTSTACK,
    _OP_FROMALTSTACK,
    _OP_2DROP,
    _OP_2DUP,
    _OP_3DUP,
    _OP_2OVER,
    _OP_2ROT,
    _OP_2SWAP,
    _OP_IFDUP,
    _OP_DEPTH,
    _OP_DROP,
    _OP_DUP,
    _OP_NIP,
    _OP_OVER,
    _OP_PICK,
    _OP_ROLL,
    _OP_ROT,
    _OP_SWAP,
    _OP_TUCK,

    // splice ops
    _OP_CAT,
    _OP_SUBSTR,
    _OP_LEFT,
    _OP_RIGHT,
    _OP_SIZE,

    // bit logic
    _OP_INVERT,
    _OP_AND,
    _OP_OR,
    _OP_XOR,
    _OP_EQUAL,
    _OP_EQUALVERIFY,
    _OP_RESERVED1,
    _OP_RESERVED2,

    // numeric
    _OP_1ADD,
    _OP_1SUB,
    _OP_2MUL,
    _OP_2DIV,
    _OP_NEGATE,
    _OP_ABS,
    _OP_NOT,
    _OP_0NOTEQUAL,

    _OP_ADD,
    _OP_SUB,
    _OP_MUL,
    _OP_DIV,
    _OP_MOD,
    _OP_LSHIFT,
    _OP_RSHIFT,

    _OP_BOOLAND,
    _OP_BOOLOR,
    _OP_NUMEQUAL,
    _OP_NUMEQUALVERIFY,
    _OP_NUMNOTEQUAL,
    _OP_LESSTHAN,
    _OP_GREATERTHAN,
    _OP_LESSTHANOREQUAL,
    _OP_GREATERTHANOREQUAL,
    _OP_MIN,
    _OP_MAX,

    _OP_WITHIN,

    // crypto
    _OP_RIPEMD160,
    _OP_SHA1,
    _OP_SHA256,
    _OP_HASH160,
    _OP_HASH256,
    _OP_CODESEPARATOR,
    _OP_CHECKSIG,
    _OP_CHECKSIGVERIFY,
    _OP_CHECKMULTISIG,
    _OP_CHECKMULTISIGVERIFY,

    // expansion
    _OP_NOP1,
    _OP_CHECKLOCKTIMEVERIFY,
    _OP_NOP2,
    _OP_CHECKSEQUENCEVERIFY,
    _OP_NOP3,
    _OP_NOP4,
    _OP_NOP5,
    _OP_NOP6,
    _OP_NOP7,
    _OP_NOP8,
    _OP_NOP9,
    _OP_NOP10,


    // template matching params
    _OP_SMALLINTEGER,
    _OP_PUBKEYS,
    _OP_PUBKEYHASH,
    _OP_PUBKEY,

    _OP_INVALIDOPCODE,
};

enum valtype {
  VT_INT=0,
  VT_DATA, // this could be an overflowed int
  VT_N
};
// UPDATE VAL_TYPE_BITS if you need more valtypes


#define VAL_TYPE_BITS 1
/* static const int COMPACT_VAL_BITS = (32-VAL_TYPE_BITS-1); */
#define COMPACT_VAL_BITS 30

struct val {
  u8 is_big : 1;
  u8 type   : VAL_TYPE_BITS;
  u32 val   : COMPACT_VAL_BITS;
};

STATIC_ASSERT(sizeof(struct val) <= 4, val_doesnt_fit_in_stack);

// Maximum value that an opcode can be
static const unsigned int MAX_OPCODE = OP_NOP10;

void         op_add(enum opcode);
const char * op_name(enum opcode);
enum opcode  op_tokenize(char *);
void         val_print(struct val);
const char * val_name(struct val);

static inline void
stack_push_val(struct stack *stack, struct val val) {
#if 0
  printf("pushing val ");
  val_print(val);
  printf("\n");
#endif
  stack_push_small(stack, &val, sizeof(struct val));
}

static inline void
stack_push_op(struct stack *stack, enum opcode opcode) {
  stack_push_small(stack, &opcode, sizeof(enum opcode));
}

static inline struct val
stack_top_val(struct stack *stack, int ind) {
  struct val val;
  void *p = stack_top(stack, ind);
  memcpy(&val, &p, sizeof(struct val));
  return val;
}

static inline void
stack_set_val(struct stack *stack, int ind, struct val val) {
  struct val *pval = (struct val *)(stack->top + ind);
  *pval = val;
}


#endif /* BCS_OP_H */
