#ifndef DICTIONARYBASE_H
#define DICTIONARYBASE_H

#include <cstdint>
#include <vector>
#include <string>
#include <cstdio>
#include "dictziplib.hpp"
#include "worddata.h"

struct CacheItem {
    uint64_t m_Offset;
    std::vector<char> m_Data;
};

#define WORDDATA_CACHE_NUM 10

class BasicDictionary {
public:
    BasicDictionary():
        m_DictFile(nullptr),
        m_CurrCacheIndex(0)
    { }

    virtual ~BasicDictionary() {
        if (m_DictFile) {
            fclose(m_DictFile);
        }
    }

    BasicDictionary(const BasicDictionary&) = delete;
    BasicDictionary &operator=(const BasicDictionary&) = delete;

public:
    void readWordData(uint64_t idxitemOffset, uint32_t idxitemSize, std::vector<char> &wordData);
#ifdef _WIN32
    bool readDictionary(const std::wstring &ifoFilepath);
#else
    bool readDictionary(const std::string &ifoFilepath);
#endif
    void unload();

private:
    bool tryGetCached(uint32_t idxitemOffset, std::vector<char> &data);

protected:
    std::string m_SameTypeSequence;
    FILE *m_DictFile;
    std::unique_ptr<DictData> m_DictDzFile;

private:
    CacheItem m_Cache[WORDDATA_CACHE_NUM];
    int m_CurrCacheIndex;
};

#endif // DICTIONARYBASE_H
