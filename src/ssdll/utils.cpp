#include "utils.h"
#include <ctype.h>
#include <cstring>

#define ISLOWER(c) ((c) >= 'a' && (c) <= 'z')
#define ISUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define TOUPPER(c) (ISLOWER (c) ? (c) - 'a' + 'A' : (c))
#define TOLOWER(c) (ISUPPER (c) ? (c) - 'A' + 'a' : (c))

typedef unsigned char guchar;

uint32_t get_uint32(const char *addr) {
    uint32_t result;
    result = *((uint32_t*)addr);
    return result;
}

uint64_t get_uint64(const char *addr) {
    uint64_t result;
    result = *((uint64_t*)addr);
    return result;
}

bool ascii_isupper(char c) {
    return isascii(c) && isupper(c);
}

// copy-paste from glib souce code
int ascii_strcasecmp(const char *s1, const char *s2) {
    int c1, c2;

    if ((s1 == nullptr) || (s2 == nullptr)) {
        return 0;
    }

    while (*s1 && *s2) {
        c1 = (int)(unsigned char) TOLOWER (*s1);
        c2 = (int)(unsigned char) TOLOWER (*s2);
        if (c1 != c2) {
            return (c1 - c2);
        }
        s1++; s2++;
    }

    return (((int)(unsigned char) *s1) - ((int)(unsigned char) *s2));
}
