
#ifndef BTCS_UTIL_H
#define BTCS_UTIL_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef intptr_t ssize_t;


int read_arg_or_stdin(const char *arg, unsigned char *buf, size_t buflen, size_t *written);
bool hex_decode(const char *str, size_t slen, void *buf, size_t bufsize);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#endif /* UTIL_H */
