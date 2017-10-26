
#ifndef BCS_MISC_H
#define BCS_MISC_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#include <stdint.h>

#define DEBUG 0

/* #define debug(fmt, ...) \ */
/*   do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0) */

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef int64_t        s64;

#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]

#endif /* BCS_MISC_H */
