#ifndef INDEXFILE_H
#define INDEXFILE_H

#include <string>
#include <cstdint>

class IIndexFile {
public:
    virtual ~IndexFile() {}

#ifdef _WIN32
    virtual bool load(const std::wstring &filepath, bool is64bitOffset) = 0;
#else
    virtual bool load(const std::string &filepath, bool is64bitOffset) = 0;
#endif
    virtual bool tryGetBounds(const std::string &word, uint64_t &offset, uint32_t &size) = 0;
};

class OrdinaryIndexFile: IIndexFile {
private:
    struct IndexEntry {
        int m_Index;
        std::string m_Key;
        void assign(int index, const std::string& key) {
            m_Index = index;
            m_Key.assign(key);
        }
    };

public:
#ifdef _WIN32
    virtual bool load(const std::wstring &filepath, bool is64bitOffset);
#else
    virtual bool load(const std::string &filepath, bool is64bitOffset);
#endif

private:
    IndexEntry m_First, m_Last, m_Middle, m_RealLast;
};

class CompressedIndexFile: IIndexFile {

};

#endif // INDEXFILE_H
