
#ifndef BCS_MISC_H
#define BCS_MISC_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* #include <endian.h> */
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define DEBUG 0

enum compiler_options {
  CO_WARNINGS_ARE_ERRORS = 1 << 1,
  CO_WARN_MINIMAL = 1 << 2
};


/* #define debug(fmt, ...) \ */
/*   do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0) */

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef int64_t        s64;
typedef uint64_t       u64;

#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]

// TODO: host endianness
#define le16toh(x) (x)
#define le32toh(x) (x)

inline static u16
readle16(const u8* ptr) {
  u16 x;
  memcpy((char*)&x, ptr, 2);
  return le16toh(x);
}

inline static u32
readle32(const u8* ptr) {
  u32 x;
  memcpy((char*)&x, ptr, 4);
  return le32toh(x);
}

inline static void
print_bytes(u8 *bytes, size_t size, int with_space) {
  size_t i;
  for (i = 0; i < size; i++) {
    printf("%02x", bytes[i]);
    if (with_space) printf(" ");
  }
}


#endif /* BCS_MISC_H */
