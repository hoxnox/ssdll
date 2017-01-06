#include "indexfile.h"
#include <cstdio>

#ifdef _WIN32
bool OrdinaryIndexFile::load(const std::wstring &filepath, bool is64bitOffset) {
#else
bool OrdinaryIndexFile::load(const std::string &filepath, bool is64bitOffset) {
#endif

}
