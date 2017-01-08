#ifndef MAPFILE_H
#define MAPFILE_H

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
    bool open(const std::wstring &filepath, size_t fileSize);
#else
    bool open(const std::string &filepath, size_t fileSize);
#endif
    char *begin() { return m_Data; }

private:
    char *m_Data;
    size_t m_Size;
    int m_mmap_fd;
};

#endif // MAPFILE_H
