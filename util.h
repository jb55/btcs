
#ifndef BTCS_UTIL_H
#define BTCS_UTIL_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


int read_arg_or_stdin(const char *arg, unsigned char *buf, size_t buflen, size_t *written);
bool hex_decode(const char *str, size_t slen, void *buf, size_t bufsize);

#endif /* UTIL_H */
