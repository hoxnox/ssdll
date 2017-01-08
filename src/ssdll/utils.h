#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

uint32_t get_uint32(const char *addr);
uint64_t get_uint64(const char *addr);
bool ascii_isupper(char c);
int ascii_strcasecmp(const char *s1, const char *s2);

#define UNUSED(x) ((void)x)

#endif // UTILS_H
