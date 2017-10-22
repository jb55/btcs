
#ifndef BCS_OPLOOKUP_H
#define BCS_OPLOOKUP_H

enum opcode_token
{
    // push value
    _OP_0,
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


enum opcode_token op_mph(const char *);


#endif /* BCS_OPLOOKUP_H */

