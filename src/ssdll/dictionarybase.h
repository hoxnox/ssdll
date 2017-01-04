#ifndef DICTIONARYBASE_H
#define DICTIONARYBASE_H

#include <cstdint>
#include <vector>
#include <string>
#include <cstdio>
#include "dictziplib.hpp"

struct CacheItem {
    uint32_t m_Offset;
    char *m_Data;
    //write code here to make it inline
    CacheItem() { m_Data = nullptr; }
    virtual ~CacheItem() {
        if (m_Data != nullptr) {
            free(m_Data);
        }
    }
};

const int WORDDATA_CACHE_NUM = 10;
const int INVALID_INDEX=-100;

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
    bool containSearchData() const {
        bool result = m_SameTypeSequence.empty() ||
                (m_SameTypeSequence.find_first_of("mlgxty") != std::string::npos);
        return result;
    }

public:
    char *GetWordData(uint32_t idxitemOffset, uint32_t idxitemSize);
    bool SearchData(std::vector<std::string> &SearchWords, uint32_t idxitemOffset, uint32_t idxitemSize, char *originData);

protected:
    std::string m_SameTypeSequence;
    FILE *m_DictFile;
    std::unique_ptr<DictData> m_DictDzFile;

private:
    CacheItem m_Cache[WORDDATA_CACHE_NUM];
    int m_CurrCacheIndex = 0;
};

#endif // DICTIONARYBASE_H
