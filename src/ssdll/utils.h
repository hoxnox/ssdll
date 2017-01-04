#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

bool ascii_isupper(char c);
inline uint32_t get_uint32(const char *addr);
inline void set_uint32(char *addr, uint32_t val);
char *strstr_len (const char *haystack,
              size_t haystack_len,
              const char *needle)

#endif // UTILS_H
