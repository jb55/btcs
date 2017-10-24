
#ifndef BTCS_SCRIPT_H
#define BTCS_SCRIPT_H

#include "op.h"

// Maximum number of bytes pushable to the stack
static const unsigned int MAX_SCRIPT_ELEMENT_SIZE = 520;

// Maximum number of non-push operations per script
static const int MAX_OPS_PER_SCRIPT = 201;

// Maximum number of public keys per multisig
static const int MAX_PUBKEYS_PER_MULTISIG = 20;

// Maximum script length in bytes
static const int MAX_SCRIPT_SIZE = 10000;

// Maximum number of values on script interpreter stack
static const int MAX_STACK_SIZE = 1000;

// Maximum value that an opcode can be
static const unsigned int MAX_OPCODE = OP_NOP10;

// Threshold for nLockTime: below this value it is interpreted as block number,
// otherwise as UNIX timestamp.
static const unsigned int LOCKTIME_THRESHOLD = 500000000; // Tue Nov  5 00:53:20 1985 UTC

struct script {
};

struct script_state {
};

void
script_eval(struct script *, struct script_state*);


#endif /* BTCS_SCRIPT_H */
