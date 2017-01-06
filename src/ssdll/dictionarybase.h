#ifndef DICTIONARYBASE_H
#define DICTIONARYBASE_H

#include <cstdint>
#include <vector>
#include <string>
#include <cstdio>
#include "dictziplib.hpp"
#include "worddata.h"

struct CacheItem {
    uint32_t m_Offset;
    std::vector<char> m_Data;
};

#define WORDDATA_CACHE_NUM 10

class DictionaryBase {
public:
    DictionaryBase():
        m_DictFile(nullptr)
    { }

    virtual ~DictionaryBase() {
        if (m_DictFile) {
            fclose(m_DictFile);
        }
    }

    DictionaryBase(const DictionaryBase&) = delete;
    DictionaryBase &operator=(const DictionaryBase&) = delete;

public:
    void readWordData(uint32_t idxitemOffset, uint32_t idxitemSize, std::vector<char> &wordData);

private:
    bool tryGetCached(uint32_t idxitemOffset, std::vector<char> &data);

protected:
    std::string m_SameTypeSequence;
    FILE *m_DictFile;
    std::unique_ptr<DictData> m_DictDzFile;

private:
    CacheItem m_Cache[WORDDATA_CACHE_NUM];
    int m_CurrCacheIndex = 0;
};

#endif // DICTIONARYBASE_H
