#include "utils.h"
#include <ctype.h>
#include <cstring>

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
