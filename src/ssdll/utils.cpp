#include "utils.h"
#include <ctype.h>
#include <cstring>

#define ISLOWER(c) ((c) >= 'a' && (c) <= 'z')
#define ISUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define TOUPPER(c) (ISLOWER (c) ? (c) - 'a' + 'A' : (c))
#define TOLOWER(c) (ISUPPER (c) ? (c) - 'A' + 'a' : (c))

typedef unsigned char guchar;

bool ascii_isupper(char c) {
    return isascii(c) && isupper(c);
}

inline uint32_t get_uint32(const char *addr) {
    uint32_t result;
    memcpy(&result, addr, sizeof(uint32_t));
    return result;
}

inline void set_uint32(char *addr, uint32_t val) {
    memcpy(addr, &val, sizeof(uint32_t));
}

// copy-paste from glib source code
char *strstr_len (const char *haystack, size_t haystack_len, const char *needle) {
    if ((haystack == nullptr) || (needle == nullptr)) {
        return nullptr;
    }

    if (haystack_len < 0) {
        return strstr (haystack, needle);
    }
    else {
        const char *p = haystack;
        size_t needle_len = strlen (needle);
        const char *end;
        size_t i;

        if (needle_len == 0) {
            return (char *)haystack;
        }

        if (haystack_len < needle_len) {
            return nullptr;
        }

        end = haystack + haystack_len - needle_len;

        while (p <= end && *p) {
            for (i = 0; i < needle_len; i++) {
                if (p[i] != needle[i]) {
                    goto next;
                }
            }

            return (char *)p;

next:
            p++;
        }

        return nullptr;
    }
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
