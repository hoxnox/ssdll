#pragma once

#ifdef _WIN32
#include <mman-win32/mman.h>
#else
#include <sys/mman.h>
#endif

#include <fcntl.h>
#include <string>

class MapFile {
public:
    MapFile():
        m_Data(nullptr),
        m_Size(0),
        m_mmap_fd(-1)
    { }

    ~MapFile();
    MapFile(const MapFile&) = delete;
    MapFile& operator=(const MapFile&) = delete;

public:
#ifdef _WIN32
    bool open(const std::wstring &filepath, unsigned long fileSize);
#else
    bool open(const std::string &filepath, unsigned long fileSize);
#endif
    char *begin() { return m_Data; }

private:
    char *m_Data;
    unsigned long m_Size;
    int m_mmap_fd;
};

#ifdef _WIN32
bool MapFile::open(const std::wstring &filepath, unsigned long fileSize) {
    if ((m_mmap_fd = _wfopen(filepath.c_str(), L"rb")) < 0) {
        return false;
    }
#else
bool MapFile::open(const std::string &filepath, unsigned long fileSize) {
    if ((m_mmap_fd = open(filepath.c_str(), "rb")) < 0) {
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

