#include "dictionaryprivate.h"
#include <cassert>
#include "utils.h"

#ifdef _WIN32
DictionaryPrivate::DictionaryPrivate(const std::wstring &ifoFilePath):
#else
DictionaryPrivate::DictionaryPrivate(const std::string &ifoFilePath):
#endif
    m_IfoFilePath(ifoFilePath),
    m_DictFile(nullptr),
    m_IsLoaded(false)
{
}

DictionaryPrivate::~DictionaryPrivate() {
    if (m_DictFile != nullptr) {
        fclose(m_DictFile);
    }
}

bool DictionaryPrivate::loadDictionary() {
    if (m_IsLoaded) { return false; }

    do {
        // TODO: tree dictionaries are not supported atm
        if (!m_DictMetadata.init(m_IfoFilePath, false)) {
            break;
        }

        if (!readDictionary()) {
            break;
        }

        if (!readIndex()) {
            break;
        }

        m_IsLoaded = true;
    } while (false);

    return m_IsLoaded;
}

bool DictionaryPrivate::readDictionary() {
    bool result = false;
    do {
#ifdef _WIN32
        std::wstring dictFilePath(m_IfoFilePath);
        dictFilePath.replace(dictFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, L"dict.dz");
#else
        std::string dictFilePath(m_IfoFilePath);
        dictFilePath.replace(dictFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, "dict.dz");
#endif
        if (fileExists(dictFilePath)) {
            m_DictGzFile.reset(new DictData());
            if (!m_DictGzFile->open(dictFilePath, false)) {
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

bool DictionaryPrivate::readIndex() {
    bool result = false;
    do {
#ifdef _WIN32
        std::wstring idxFilePath(m_IfoFilePath);
        idxFilePath.replace(idxFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, L"idx.dz");
#else
        std::string idxFilePath(m_IfoFilePath);
        idxFilePath.replace(idxFilePath.length() - sizeof("ifo") + 1, sizeof("ifo") - 1, "idx.dz");
#endif

        unsigned long long fileSize;
        if (fileExists(idxFilePath, fileSize)) {
            m_IndexFile.reset(new CompressedIndexFile());
        } else {
            idxFilePath.erase(idxFilePath.length() - sizeof(".gz") + 1, sizeof(".gz") - 1);

            if (!fileExists(idxFilePath, fileSize)) {
                break;
            }

            m_IndexFile.reset(new OrdinaryIndexFile());
        }

        assert(fileSize == m_DictMetadata.getIndexFileSize());

        if (!m_IndexFile->load(idxFilePath,
                               m_DictMetadata.isOffset64Bit(),
                               m_DictMetadata.getWordCount(),
                               fileSize)) {
            break;
        }

        result = true;
    } while (false);

    return result;
}
