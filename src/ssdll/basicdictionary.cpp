#include "basicdictionary.h"
#include <cassert>
#include <cstring>
#include "utils.h"

void BasicDictionary::readWordData(uint64_t idxitemOffset, uint32_t idxitemSize, std::vector<char> &wordData) {
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

#ifdef _WIN32
bool DictionaryBase::readDictionary(const std::string &ifoFilepath) {
    bool result = false;
    std::wstring dictFilePath(ifoFilepath);
    dictFilePath.replace(dictFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, L"dict.dz");
#else
bool BasicDictionary::readDictionary(const std::string &ifoFilepath) {
    bool result = false;
    std::string dictFilePath(ifoFilepath);
    dictFilePath.replace(dictFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, "dict.dz");
#endif
    do {
        if (fileExists(dictFilePath)) {
            m_DictDzFile.reset(new DictData());
            if (!m_DictDzFile->open(dictFilePath, false)) {
                break;
            }
        } else {
#ifdef _WIN32
            m_DictFile = _wfopen(dictFilePath.c_str(), L"rb");
#else
            m_DictFile = fopen(dictFilePath.c_str(), "rb");
#endif
            if (!m_DictFile) {
                break;
            }
        }

        result = true;
    } while (false);

    return result;
}

void BasicDictionary::unload() {
    if (m_DictFile != nullptr) {
        fclose(m_DictFile);
        m_DictFile = nullptr;
    }

    m_DictDzFile.reset();

    for (auto &c: m_Cache) {
        c.m_Data.clear();
        c.m_Offset = 0;
    }

    m_CurrCacheIndex = 0;
}

bool BasicDictionary::tryGetCached(uint32_t idxitemOffset, std::vector<char> &data) {
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
