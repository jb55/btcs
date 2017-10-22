
#include "script.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>



enum opcode
op_tokenize(char *str) {
  int len,i;
  char *p;

  p = str;
  len = strlen(str);

  for (i = 0; i < len; ++i)
    str[i] = toupper(str[i]);

  // upper all all
  if (len >= 3)
  if (str[0] == 'O')
  if (str[1] == 'P')
  if (str[2] == '_') {
    p = &str[3];
    len -= 3;
  }

  return op_mph(str);
}


enum opcode op_from_token(enum opcode_token opcode) {
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
}


const char* op_str(enum opcode opcode)
{
    switch (opcode)
    {
    // push value
    case OP_0                      : return "0";
    case OP_PUSHDATA1              : return "OP_PUSHDATA1";
    case OP_PUSHDATA2              : return "OP_PUSHDATA2";
    case OP_PUSHDATA4              : return "OP_PUSHDATA4";
    case OP_1NEGATE                : return "-1";
    case OP_RESERVED               : return "OP_RESERVED";
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
    case OP_NOP                    : return "OP_NOP";
    case OP_VER                    : return "OP_VER";
    case OP_IF                     : return "OP_IF";
    case OP_NOTIF                  : return "OP_NOTIF";
    case OP_VERIF                  : return "OP_VERIF";
    case OP_VERNOTIF               : return "OP_VERNOTIF";
    case OP_ELSE                   : return "OP_ELSE";
    case OP_ENDIF                  : return "OP_ENDIF";
    case OP_VERIFY                 : return "OP_VERIFY";
    case OP_RETURN                 : return "OP_RETURN";

    // stack ops
    case OP_TOALTSTACK             : return "OP_TOALTSTACK";
    case OP_FROMALTSTACK           : return "OP_FROMALTSTACK";
    case OP_2DROP                  : return "OP_2DROP";
    case OP_2DUP                   : return "OP_2DUP";
    case OP_3DUP                   : return "OP_3DUP";
    case OP_2OVER                  : return "OP_2OVER";
    case OP_2ROT                   : return "OP_2ROT";
    case OP_2SWAP                  : return "OP_2SWAP";
    case OP_IFDUP                  : return "OP_IFDUP";
    case OP_DEPTH                  : return "OP_DEPTH";
    case OP_DROP                   : return "OP_DROP";
    case OP_DUP                    : return "OP_DUP";
    case OP_NIP                    : return "OP_NIP";
    case OP_OVER                   : return "OP_OVER";
    case OP_PICK                   : return "OP_PICK";
    case OP_ROLL                   : return "OP_ROLL";
    case OP_ROT                    : return "OP_ROT";
    case OP_SWAP                   : return "OP_SWAP";
    case OP_TUCK                   : return "OP_TUCK";

    // splice ops
    case OP_CAT                    : return "OP_CAT";
    case OP_NOT                    : return "OP_NOT";
    case OP_0NOTEQUAL              : return "OP_0NOTEQUAL";
    case OP_ADD                    : return "OP_ADD";
    case OP_SUB                    : return "OP_SUB";
    case OP_MUL                    : return "OP_MUL";
    case OP_DIV                    : return "OP_DIV";
    case OP_MOD                    : return "OP_MOD";
    case OP_LSHIFT                 : return "OP_LSHIFT";
    case OP_RSHIFT                 : return "OP_RSHIFT";
    case OP_BOOLAND                : return "OP_BOOLAND";
    case OP_BOOLOR                 : return "OP_BOOLOR";
    case OP_NUMEQUAL               : return "OP_NUMEQUAL";
    case OP_NUMEQUALVERIFY         : return "OP_NUMEQUALVERIFY";
    case OP_NUMNOTEQUAL            : return "OP_NUMNOTEQUAL";
    case OP_LESSTHAN               : return "OP_LESSTHAN";
    case OP_GREATERTHAN            : return "OP_GREATERTHAN";
    case OP_LESSTHANOREQUAL        : return "OP_LESSTHANOREQUAL";
    case OP_GREATERTHANOREQUAL     : return "OP_GREATERTHANOREQUAL";
    case OP_MIN                    : return "OP_MIN";
    case OP_MAX                    : return "OP_MAX";
    case OP_WITHIN                 : return "OP_WITHIN";

    // crypto
    case OP_RIPEMD160              : return "OP_RIPEMD160";
    case OP_SHA1                   : return "OP_SHA1";
    case OP_SHA256                 : return "OP_SHA256";
    case OP_HASH160                : return "OP_HASH160";
    case OP_HASH256                : return "OP_HASH256";
    case OP_CODESEPARATOR          : return "OP_CODESEPARATOR";
    case OP_CHECKSIG               : return "OP_CHECKSIG";
    case OP_CHECKSIGVERIFY         : return "OP_CHECKSIGVERIFY";
    case OP_CHECKMULTISIG          : return "OP_CHECKMULTISIG";
    case OP_CHECKMULTISIGVERIFY    : return "OP_CHECKMULTISIGVERIFY";

    // expansion
    case OP_NOP1                   : return "OP_NOP1";
    case OP_CHECKLOCKTIMEVERIFY    : return "OP_CHECKLOCKTIMEVERIFY";
    case OP_CHECKSEQUENCEVERIFY    : return "OP_CHECKSEQUENCEVERIFY";
    case OP_NOP4                   : return "OP_NOP4";
    case OP_NOP5                   : return "OP_NOP5";
    case OP_NOP6                   : return "OP_NOP6";
    case OP_NOP7                   : return "OP_NOP7";
    case OP_NOP8                   : return "OP_NOP8";
    case OP_NOP9                   : return "OP_NOP9";
    case OP_NOP10                  : return "OP_NOP10";

    case OP_INVALIDOPCODE          : return "OP_INVALIDOPCODE";

    // Note:
    //  The template matching params OP_SMALLINTEGER/etc are defined in opcodetype enum
    //  as kind of implementation hack, they are *NOT* real opcodes.  If found in real
    //  Script, just let the default: case deal with them.

    default:
        return "OP_UNKNOWN";
    }
}


