#include "dictionarybase.h"
#include <cassert>
#include <cstring>
#include "utils.h"

void DictionaryBase::readWordData(uint32_t idxitemOffset, uint32_t idxitemSize, std::vector<char> &wordData) {
    if (tryGetCached(idxitemOffset, wordData)) {
        return;
    }

    wordData.resize(idxitemSize, '\0');
    char *buffer = &wordData[0];

    if (m_DictFile) {
        fseek(m_DictFile, idxitemOffset, SEEK_SET);
        size_t nitems = fread(buffer, idxitemSize, 1, m_DictFile);
        assert(nitems == 1);
    } else {
        m_DictDzFile->read(buffer, idxitemOffset, idxitemSize);
    }

    m_Cache[m_CurrCacheIndex].m_Data = wordData;
    m_Cache[m_CurrCacheIndex].m_Offset = idxitemOffset;
    m_CurrCacheIndex++;

    if (m_CurrCacheIndex == WORDDATA_CACHE_NUM) {
        m_CurrCacheIndex = 0;
    }
}

bool DictionaryBase::tryGetCached(uint32_t idxitemOffset, std::vector<char> &data) {
    bool found = false;

    for (int i = 0; i < WORDDATA_CACHE_NUM; i++) {
        if ((!m_Cache[i].m_Data.empty()) && (m_Cache[i].m_Offset == idxitemOffset)) {
            data = m_Cache[i].m_Data;
            found = true;
            break;
        }
    }

    return found;
}
