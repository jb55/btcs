
#include "util.h"
#include <errno.h>
#include <assert.h>

static bool char_to_hex(unsigned char *val, char c)
{
	if (c >= '0' && c <= '9') {
		*val = c - '0';
		return true;
	}
 	if (c >= 'a' && c <= 'f') {
		*val = c - 'a' + 10;
		return true;
	}
 	if (c >= 'A' && c <= 'F') {
		*val = c - 'A' + 10;
		return true;
	}
	return false;
}

bool hex_decode(const char *str, size_t *slen, void *buf, size_t bufsize)
{
	unsigned char v1, v2;
	unsigned char *p = buf;
	int len = 0;

	while (*slen > 1) {
		if (!bufsize || !char_to_hex(&v1, str[0]) || !char_to_hex(&v2, str[1])) {
			*slen = len;
			return false;
		}
		*(p++) = (v1 << 4) | v2;
		str += 2;
		*slen -= 2;
		bufsize--;
		len += 2;
	}
	*slen = len;
	return *slen == 0 && bufsize == 0;
}


int read_fd(FILE *fd, unsigned char *buf, size_t buflen, size_t *written)
{
	unsigned char *p = buf;
	int len = 0;
	*written = 0;

	do {
		len = fread(p, 1, 4096, fd);
		*written += len;
		p += len;
		if (p > buf + buflen)
			return 0;
	} while (len == 4096);

	return 1;
}

	

// if typedef doesn't exist (msvc, blah)
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

int read_arg_or_stdin(const char *arg, unsigned char *buf, size_t buflen,
		      size_t *written)
{
	if (arg != NULL) {
		unsigned char *p = buf;
		bool done = false;
		for (size_t i = 0; i < buflen; i++) {
			*p = arg[i];
			if (*p == 0 ) {
				done = true;
				break;
			}
			p++;
		}

		*written = p - buf;

		return done;
	}
	else {
		int ok = read_fd(stdin, buf, buflen, written);
		if (!ok)
			return ok;
		if (*written == 0)
			return 0;
		(*written)--;
		buf[*written] = 0;
		return ok;
	}
}

