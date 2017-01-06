#ifndef DICTIONARYBASE_H
#define DICTIONARYBASE_H

#include <cstdint>
#include <vector>
#include <string>
#include <cstdio>
#include "dictziplib.hpp"

struct CacheItem {
    uint32_t m_Offset;
    std::string m_Data;
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
    std::string getWordData(uint32_t idxitemOffset, uint32_t idxitemSize);
    bool hasMatchInData(const std::vector<std::string> &SearchWords, uint32_t idxitemOffset, uint32_t idxitemSize, std::vector &placeHolderForData);

protected:
    std::string m_SameTypeSequence;
    FILE *m_DictFile;
    std::unique_ptr<DictData> m_DictDzFile;

private:
    CacheItem m_Cache[WORDDATA_CACHE_NUM];
    int m_CurrCacheIndex = 0;
};

#endif // DICTIONARYBASE_H
