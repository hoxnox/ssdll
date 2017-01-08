#ifndef INDEXFILE_H
#define INDEXFILE_H

#include <string>
#include <cstdint>
#include <vector>

class IIndexFile {
public:
    virtual ~IIndexFile() {}

#ifdef _WIN32
    virtual bool load(const std::wstring &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize) = 0;
#else
    virtual bool load(const std::string &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize) = 0;
#endif
    virtual bool findBounds(const std::string &word, uint64_t &offset, uint32_t &size) = 0;
    virtual std::string retrieveKey(int index, uint64_t &offset, uint32_t &size) = 0;
};

class OrdinaryIndexFile: IIndexFile {
public:
    OrdinaryIndexFile();
    virtual ~OrdinaryIndexFile();

private:
    static const int ENTR_PER_PAGE = 32;
    static const char *CACHE_MAGIC;

    struct IndexEntry {
        int m_Index;
        std::string m_Key;
        void assign(int index, const std::string& key) {
            m_Index = index;
            m_Key.assign(key);
        }
    };

    struct PageEntry {
        char *KeyString;
        uint64_t m_Offset;
        uint32_t m_Size;
    };

    struct CachedPage {
        int m_Index = -1;
        PageEntry m_Entries[ENTR_PER_PAGE];

        CachedPage(): m_Index(-1) {}
        void parseIndexData(char *data, int nent, long index, bool is64Offset);
    };

public:
#ifdef _WIN32
    virtual bool load(const std::wstring &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize);
#else
    virtual bool load(const std::string &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize);
#endif
    virtual bool findBounds(const std::string &word, uint64_t &offset, uint32_t &size);

private:
#ifdef _WIN32
    bool createCache(const std::wstring &idxfilepath, size_t fileSize);
    bool loadCache(const std::wstring &idxfilepath);
    void saveCache(const std::wstring &idxfilepath);
#else
    bool createCache(const std::string &idxfilepath, size_t fileSize);
    bool loadCache(const std::string &idxfilepath);
    void saveCache(const std::string &idxfilepath);
#endif
    int loadPage(int index);
    char *readFirstKeyOnPage(int pageIndex);
    const char *retrieveFirstKeyOnPage(int pageIndex);
    virtual std::string retrieveKey(int index, uint64_t &offset, uint32_t &size);
    std::string retrieveKey(int index);
    bool findThroughPages(const char *key, int &index);
    bool findInPage(const char *key, int &index);

private:
    std::vector<uint64_t> m_WordOffset;
    FILE *m_IndexFile;
    unsigned int m_WordCount;
    char m_WordEntryBuffer[256+sizeof(uint32_t)*3]; // The length of "word_str" should be less than 256. See src/tools/DICTFILE_FORMAT.
    IndexEntry m_First, m_Last, m_Middle, m_RealLast;
    std::vector<char> m_PageData;
    CachedPage m_Page;
    bool m_Is64BitOffset;
};

class CompressedIndexFile: IIndexFile {
public:
    CompressedIndexFile();

public:
#ifdef _WIN32
    virtual bool load(const std::wstring &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize);
#else
    virtual bool load(const std::string &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize);
#endif
    virtual bool findBounds(const std::string &word, uint64_t &offset, uint32_t &size);

private:
    bool findInWords(const char *key, int &index);

private:
    std::vector<char> m_IdxDataBuffer;
    std::vector<char *> m_WordList;
    bool m_Is64BitOffset;
};

#endif // INDEXFILE_H
