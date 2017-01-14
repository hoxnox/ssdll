#include "mapfile.hpp"
#ifdef _WIN32
#include <mman-win32/mman.h>
#include <io.h>
#else
#include <sys/mman.h>
#endif

#include <cstdio>
#include <fcntl.h>

#ifdef _WIN32
bool MapFile::open(const std::wstring &filepath, size_t fileSize) {
    if ((m_mmap_fd = _wopen(filepath.c_str(), O_RDONLY)) < 0) {
        return false;
    }
#else
bool MapFile::open(const std::string &filepath, size_t fileSize) {
    if ((m_mmap_fd = ::open(filepath.c_str(), O_RDONLY)) < 0) {
        return false;
    }
#endif

    m_Size = fileSize;

    m_Data = (char*) mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, m_mmap_fd, 0);
    if ((void *)m_Data == (void *)(-1)) {
        //g_print("mmap file %s failed!\n",idxfilename);
        m_Data = nullptr;
        return false;
    }

    return true;
}

MapFile::~MapFile() {
    if (m_Data != nullptr) {
        munmap(m_Data, m_Size);
        close(m_mmap_fd);
    }
}
