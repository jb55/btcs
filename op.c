
#include "script.h"
#include "oplookup.h"
#include "op.h"
#include "alloc.h"
#include "misc.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>



static enum opcode
op_from_token(enum opcode_token opcode) {
  switch (opcode) {
  case _OP_0: return OP_0;
  case _OP_FALSE: return OP_FALSE;
  case _OP_PUSHDATA1: return OP_PUSHDATA1;
  case _OP_PUSHDATA2: return OP_PUSHDATA2;
  case _OP_PUSHDATA4: return OP_PUSHDATA4;
  case _OP_1NEGATE: return OP_1NEGATE;
  case _OP_RESERVED: return OP_RESERVED;
  case _OP_1: return OP_1;
  case _OP_TRUE: return OP_TRUE;
  case _OP_2: return OP_2;
  case _OP_3: return OP_3;
  case _OP_4: return OP_4;
  case _OP_5: return OP_5;
  case _OP_6: return OP_6;
  case _OP_7: return OP_7;
  case _OP_8: return OP_8;
  case _OP_9: return OP_9;
  case _OP_10: return OP_10;
  case _OP_11: return OP_11;
  case _OP_12: return OP_12;
  case _OP_13: return OP_13;
  case _OP_14: return OP_14;
  case _OP_15: return OP_15;
  case _OP_16: return OP_16;
  case _OP_NOP: return OP_NOP;
  case _OP_VER: return OP_VER;
  case _OP_IF: return OP_IF;
  case _OP_NOTIF: return OP_NOTIF;
  case _OP_VERIF: return OP_VERIF;
  case _OP_VERNOTIF: return OP_VERNOTIF;
  case _OP_ELSE: return OP_ELSE;
  case _OP_ENDIF: return OP_ENDIF;
  case _OP_VERIFY: return OP_VERIFY;
  case _OP_RETURN: return OP_RETURN;
  case _OP_TOALTSTACK: return OP_TOALTSTACK;
  case _OP_FROMALTSTACK: return OP_FROMALTSTACK;
  case _OP_2DROP: return OP_2DROP;
  case _OP_2DUP: return OP_2DUP;
  case _OP_3DUP: return OP_3DUP;
  case _OP_2OVER: return OP_2OVER;
  case _OP_2ROT: return OP_2ROT;
  case _OP_2SWAP: return OP_2SWAP;
  case _OP_IFDUP: return OP_IFDUP;
  case _OP_DEPTH: return OP_DEPTH;
  case _OP_DROP: return OP_DROP;
  case _OP_DUP: return OP_DUP;
  case _OP_NIP: return OP_NIP;
  case _OP_OVER: return OP_OVER;
  case _OP_PICK: return OP_PICK;
  case _OP_ROLL: return OP_ROLL;
  case _OP_ROT: return OP_ROT;
  case _OP_SWAP: return OP_SWAP;
  case _OP_TUCK: return OP_TUCK;
  case _OP_CAT: return OP_CAT;
  case _OP_SUBSTR: return OP_SUBSTR;
  case _OP_LEFT: return OP_LEFT;
  case _OP_RIGHT: return OP_RIGHT;
  case _OP_SIZE: return OP_SIZE;
  case _OP_INVERT: return OP_INVERT;
  case _OP_AND: return OP_AND;
  case _OP_OR: return OP_OR;
  case _OP_XOR: return OP_XOR;
  case _OP_EQUAL: return OP_EQUAL;
  case _OP_EQUALVERIFY: return OP_EQUALVERIFY;
  case _OP_RESERVED1: return OP_RESERVED1;
  case _OP_RESERVED2: return OP_RESERVED2;
  case _OP_1ADD: return OP_1ADD;
  case _OP_1SUB: return OP_1SUB;
  case _OP_2MUL: return OP_2MUL;
  case _OP_2DIV: return OP_2DIV;
  case _OP_NEGATE: return OP_NEGATE;
  case _OP_ABS: return OP_ABS;
  case _OP_NOT: return OP_NOT;
  case _OP_0NOTEQUAL: return OP_0NOTEQUAL;
  case _OP_ADD: return OP_ADD;
  case _OP_SUB: return OP_SUB;
  case _OP_MUL: return OP_MUL;
  case _OP_DIV: return OP_DIV;
  case _OP_MOD: return OP_MOD;
  case _OP_LSHIFT: return OP_LSHIFT;
  case _OP_RSHIFT: return OP_RSHIFT;
  case _OP_BOOLAND: return OP_BOOLAND;
  case _OP_BOOLOR: return OP_BOOLOR;
  case _OP_NUMEQUAL: return OP_NUMEQUAL;
  case _OP_NUMEQUALVERIFY: return OP_NUMEQUALVERIFY;
  case _OP_NUMNOTEQUAL: return OP_NUMNOTEQUAL;
  case _OP_LESSTHAN: return OP_LESSTHAN;
  case _OP_GREATERTHAN: return OP_GREATERTHAN;
  case _OP_LESSTHANOREQUAL: return OP_LESSTHANOREQUAL;
  case _OP_GREATERTHANOREQUAL: return OP_GREATERTHANOREQUAL;
  case _OP_MIN: return OP_MIN;
  case _OP_MAX: return OP_MAX;
  case _OP_WITHIN: return OP_WITHIN;
  case _OP_RIPEMD160: return OP_RIPEMD160;
  case _OP_SHA1: return OP_SHA1;
  case _OP_SHA256: return OP_SHA256;
  case _OP_HASH160: return OP_HASH160;
  case _OP_HASH256: return OP_HASH256;
  case _OP_CODESEPARATOR: return OP_CODESEPARATOR;
  case _OP_CHECKSIG: return OP_CHECKSIG;
  case _OP_CHECKSIGVERIFY: return OP_CHECKSIGVERIFY;
  case _OP_CHECKMULTISIG: return OP_CHECKMULTISIG;
  case _OP_CHECKMULTISIGVERIFY: return OP_CHECKMULTISIGVERIFY;
  case _OP_NOP1: return OP_NOP1;
  case _OP_CHECKLOCKTIMEVERIFY: return OP_CHECKLOCKTIMEVERIFY;
  case _OP_NOP2: return OP_NOP2;
  case _OP_CHECKSEQUENCEVERIFY: return OP_CHECKSEQUENCEVERIFY;
  case _OP_NOP3: return OP_NOP3;
  case _OP_NOP4: return OP_NOP4;
  case _OP_NOP5: return OP_NOP5;
  case _OP_NOP6: return OP_NOP6;
  case _OP_NOP7: return OP_NOP7;
  case _OP_NOP8: return OP_NOP8;
  case _OP_NOP9: return OP_NOP9;
  case _OP_NOP10: return OP_NOP10;
  case _OP_SMALLINTEGER: return OP_SMALLINTEGER;
  case _OP_PUBKEYS: return OP_PUBKEYS;
  case _OP_PUBKEYHASH: return OP_PUBKEYHASH;
  case _OP_PUBKEY: return OP_PUBKEY;
  case _OP_INVALIDOPCODE: return OP_INVALIDOPCODE;
  }

  return OP_INVALIDOPCODE;
}

const char*
op_name(enum opcode opcode)
{
    switch (opcode)
    {
    // push value
    case OP_0                      : return "0";
    case OP_PUSHDATA1              : return "PUSHDATA1";
    case OP_PUSHDATA2              : return "PUSHDATA2";
    case OP_PUSHDATA4              : return "PUSHDATA4";
    case OP_1NEGATE                : return "-1";
    case OP_RESERVED               : return "RESERVED";
    case OP_1                      : return "1";
    case OP_2                      : return "2";
    case OP_3                      : return "3";
    case OP_4                      : return "4";
    case OP_5                      : return "5";
    case OP_6                      : return "6";
    case OP_7                      : return "7";
    case OP_8                      : return "8";
    case OP_9                      : return "9";
    case OP_10                     : return "10";
    case OP_11                     : return "11";
    case OP_12                     : return "12";
    case OP_13                     : return "13";
    case OP_14                     : return "14";
    case OP_15                     : return "15";
    case OP_16                     : return "16";

    // control
    case OP_NOP                    : return "NOP";
    case OP_VER                    : return "VER";
    case OP_IF                     : return "IF";
    case OP_NOTIF                  : return "NOTIF";
    case OP_VERIF                  : return "VERIF";
    case OP_VERNOTIF               : return "VERNOTIF";
    case OP_ELSE                   : return "ELSE";
    case OP_ENDIF                  : return "ENDIF";
    case OP_VERIFY                 : return "VERIFY";
    case OP_RETURN                 : return "RETURN";

    // stack ops
    case OP_TOALTSTACK             : return "TOALTSTACK";
    case OP_FROMALTSTACK           : return "FROMALTSTACK";
    case OP_2DROP                  : return "2DROP";
    case OP_2DUP                   : return "2DUP";
    case OP_3DUP                   : return "3DUP";
    case OP_2OVER                  : return "2OVER";
    case OP_2ROT                   : return "2ROT";
    case OP_2SWAP                  : return "2SWAP";
    case OP_IFDUP                  : return "IFDUP";
    case OP_DEPTH                  : return "DEPTH";
    case OP_DROP                   : return "DROP";
    case OP_DUP                    : return "DUP";
    case OP_NIP                    : return "NIP";
    case OP_OVER                   : return "OVER";
    case OP_PICK                   : return "PICK";
    case OP_ROLL                   : return "ROLL";
    case OP_ROT                    : return "ROT";
    case OP_SWAP                   : return "SWAP";
    case OP_TUCK                   : return "TUCK";

    // splice ops
    case OP_CAT                    : return "CAT";
    case OP_SUBSTR                 : return "SUBSTR";
    case OP_LEFT                   : return "LEFT";
    case OP_RIGHT                  : return "RIGHT";
    case OP_SIZE                   : return "SIZE";

    // bit logic
    case OP_INVERT                 : return "INVERT";
    case OP_AND                    : return "AND";
    case OP_OR                     : return "OR";
    case OP_XOR                    : return "XOR";
    case OP_EQUAL                  : return "EQUAL";
    case OP_EQUALVERIFY            : return "EQUALVERIFY";
    case OP_RESERVED1              : return "RESERVED1";
    case OP_RESERVED2              : return "RESERVED2";

    // numeric
    case OP_1ADD                   : return "1ADD";
    case OP_1SUB                   : return "1SUB";
    case OP_2MUL                   : return "2MUL";
    case OP_2DIV                   : return "2DIV";
    case OP_NEGATE                 : return "NEGATE";
    case OP_ABS                    : return "ABS";
    case OP_NOT                    : return "NOT";
    case OP_0NOTEQUAL              : return "0NOTEQUAL";
    case OP_ADD                    : return "ADD";
    case OP_SUB                    : return "SUB";
    case OP_MUL                    : return "MUL";
    case OP_DIV                    : return "DIV";
    case OP_MOD                    : return "MOD";
    case OP_LSHIFT                 : return "LSHIFT";
    case OP_RSHIFT                 : return "RSHIFT";
    case OP_BOOLAND                : return "BOOLAND";
    case OP_BOOLOR                 : return "BOOLOR";
    case OP_NUMEQUAL               : return "NUMEQUAL";
    case OP_NUMEQUALVERIFY         : return "NUMEQUALVERIFY";
    case OP_NUMNOTEQUAL            : return "NUMNOTEQUAL";
    case OP_LESSTHAN               : return "LESSTHAN";
    case OP_GREATERTHAN            : return "GREATERTHAN";
    case OP_LESSTHANOREQUAL        : return "LESSTHANOREQUAL";
    case OP_GREATERTHANOREQUAL     : return "GREATERTHANOREQUAL";
    case OP_MIN                    : return "MIN";
    case OP_MAX                    : return "MAX";
    case OP_WITHIN                 : return "WITHIN";

    // crypto
    case OP_RIPEMD160              : return "RIPEMD160";
    case OP_SHA1                   : return "SHA1";
    case OP_SHA256                 : return "SHA256";
    case OP_HASH160                : return "HASH160";
    case OP_HASH256                : return "HASH256";
    case OP_CODESEPARATOR          : return "CODESEPARATOR";
    case OP_CHECKSIG               : return "CHECKSIG";
    case OP_CHECKSIGVERIFY         : return "CHECKSIGVERIFY";
    case OP_CHECKMULTISIG          : return "CHECKMULTISIG";
    case OP_CHECKMULTISIGVERIFY    : return "CHECKMULTISIGVERIFY";

    // expansion
    case OP_NOP1                   : return "NOP1";
    case OP_CHECKLOCKTIMEVERIFY    : return "CHECKLOCKTIMEVERIFY";
    case OP_CHECKSEQUENCEVERIFY    : return "CHECKSEQUENCEVERIFY";
    case OP_NOP4                   : return "NOP4";
    case OP_NOP5                   : return "NOP5";
    case OP_NOP6                   : return "NOP6";
    case OP_NOP7                   : return "NOP7";
    case OP_NOP8                   : return "NOP8";
    case OP_NOP9                   : return "NOP9";
    case OP_NOP10                  : return "NOP10";

    case OP_INVALIDOPCODE          : return "INVALIDOPCODE";

    // Note:
    //  The template matching params OP_SMALLINTEGER/etc are defined in opcodetype enum
    //  as kind of implementation hack, they are *NOT* real opcodes.  If found in real
    //  Script, just let the default: case deal with them.

    default: {
      static char buf[32];
      sprintf(buf, "%02x", opcode);
      return buf;
    }
    }
}




static u32
hash_fnv(u32 d, const char *str) {
  u8 *p = (u8*)str;

  if (d == 0)
    d = 0x01000193;

  while(*p)
    d = ((d * 0x01000193) ^ *p++) & 0xffffffff;

  return d;
}


enum opcode_token
op_hash(const char *str) {
  int d = opcodes_g[hash_fnv(0, str) % ARRAY_SIZE(opcodes_g)];
  if (d < 0) d = opcodes_v[-d-1];
  else       d = opcodes_v[hash_fnv(d, str) % ARRAY_SIZE(opcodes_v)];
  return d;
}



const char *
op_normalized_tok(const char *tok, int hash) {
  if (hash != _OP_TRUE  &&
      hash != _OP_FALSE &&
      hash != _OP_NOP2  &&
      hash != _OP_NOP3) return tok;

  if (strcmp(tok, "TRUE") == 0) return "1";
  else if (strcmp(tok, "FALSE") == 0) return "0";
  else if (strcmp(tok, "NOP2") == 0) return "CHECKLOCKTIMEVERIFY";
  else if (strcmp(tok, "NOP3") == 0) return "CHECKSEQUENCEVERIFY";

  return tok;
}


enum opcode
op_parse_opcode(const char *str) {
  int hash     = op_hash(str);
  int maybe_op = op_from_token(hash);

  if (strcmp(op_name(maybe_op), op_normalized_tok(str, hash)) == 0)
    return maybe_op;
  else
    return OP_INVALIDOPCODE;
}


enum opcode
op_tokenize(char *str) {
  int i;

  for (i = 0; str[i]; ++i)
    str[i] = toupper(str[i]);

  return op_parse_opcode(str);
}


void
val_print(struct val val) {
  struct num *n;
  switch(val.type) {
  case VT_SCRIPTNUM:
    assert(val.ind != -1);
    n = num_pool_get(val.ind);
    assert(n);
    printf("%" PRId64, n->val);
    break;
  case VT_OP:
    printf("OP_%s", op_name(val.ind));
    break;
  case VT_SMALLINT:
    printf("%d", val.ind);
    break;
  case VT_DATA: {
    u32 len;
    u8 *data = byte_pool_get(val.ind, &len);

    if (len == 0)
      printf("0", len);
    else
      printf("@", len);

    print_bytes(data, len, 0);
    break;
  }
  default:
    assert(!"val_print data");
  }
}

const char *
val_name(struct val val) {
  switch(val.type) {
  case VT_SCRIPTNUM:  return "n"; break;
  case VT_SMALLINT:  return "s"; break;
  case VT_DATA: return "d"; break;
  }
  return "UNK_VAL";
}
