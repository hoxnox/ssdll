#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>

uint32_t get_uint32(const char *addr);
uint64_t get_uint64(const char *addr);
bool ascii_isupper(char c);
int ascii_strcasecmp(const char *s1, const char *s2);

#ifdef _WIN32
bool fileExists(const std::wstring &path);
bool fileExists(const std::wstring &path, unsigned long long &fileSize);
#else
bool fileExists(const std::string &path);
bool fileExists(const std::string &path, unsigned long long &fileSize);
#endif

#define UNUSED(x) ((void)x)

#endif // UTILS_H
