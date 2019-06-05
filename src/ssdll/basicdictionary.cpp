#include "basicdictionary.h"
#include <cassert>
#include <cstring>

#include "utils.h"
#include "logger.h"

BasicDictionary::~BasicDictionary() {
    LOG << "BasicDictionary::~BasicDictionary - #";
    unload();
}

void BasicDictionary::readWordData(uint64_t idxitemOffset, uint32_t idxitemSize, std::vector<char> &wordData) {
    if (tryGetCached(idxitemOffset, wordData)) {
        return;
    }

    wordData.resize(idxitemSize, '\0');
    char *buffer = &wordData[0];

    if (m_DictFile) {
#ifdef _WIN32
        _fseeki64(m_DictFile, idxitemOffset, SEEK_SET);
#else
        fseek(m_DictFile, idxitemOffset, SEEK_SET);
#endif
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
bool BasicDictionary::readDictionary(const std::wstring &ifoFilepath) {
    bool result = false;
    std::wstring dictFilePath(ifoFilepath);
    dictFilePath.replace(dictFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, L"dict.dz");
#else
bool BasicDictionary::readDictionary(const std::string &ifoFilepath) {
    bool result = false;
    std::string dictFilePath(ifoFilepath);
    dictFilePath.replace(dictFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, "dict.dz");
#endif
    LOG << "BasicDictionary::readDictionary -" << ifoFilepath;
    do {
        if (fileExists(dictFilePath)) {
            m_DictDzFile = std::make_unique<DictData>();
            if (!m_DictDzFile->open(dictFilePath, false)) {
                LOG << "BasicDictionary::readDictionary - failed to open as .DICT.DZ" << dictFilePath;
                break;
            }
        } else {
#ifdef _WIN32
            m_DictFile = _wfopen(dictFilePath.c_str(), L"rb");
#else
            m_DictFile = fopen(dictFilePath.c_str(), "rb");
#endif
            if (!m_DictFile) {
                LOG << "BasicDictionary::readDictionary - failed to open" << dictFilePath;
                break;
            }
        }

        result = true;
    } while (false);

    return result;
}

void BasicDictionary::unload() {
    LOG << "BasicDictionary::unload - #";
    if (m_DictFile != nullptr) {
        LOG << "BasicDictionary::unload - closing dict file";
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

bool BasicDictionary::tryGetCached(uint64_t idxitemOffset, std::vector<char> &data) {
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
