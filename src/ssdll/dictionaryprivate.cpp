#include "dictionaryprivate.h"
#include <cassert>
#include "utils.h"

#ifdef _WIN32
DictionaryPrivate::DictionaryPrivate(const std::wstring &ifoFilePath):
#else
DictionaryPrivate::DictionaryPrivate(const std::string &ifoFilePath):
#endif
    m_IfoFilePath(ifoFilePath),
    m_IsLoaded(false)
{
}

DictionaryPrivate::~DictionaryPrivate() {
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

void DictionaryPrivate::unloadDictionary() {
    m_IfoFilePath.clear();
    m_DictMetadata.clear();
    m_IndexFile.reset();
    m_BasicDictionary.unload();
    m_IsLoaded = true;
}

bool DictionaryPrivate::findPureMeaning(const std::string &word, std::string &meaning) {
    if (!m_IsLoaded) { return false; }
    bool result = false;

    uint64_t offset = 0;
    uint32_t size = 0;

    if (m_IndexFile->findBounds(word, offset, size)) {
        std::vector<char> rawWordData;
        m_BasicDictionary.readWordData(offset, size, rawWordData);

        WordData wordData;
        if (wordData.parse(rawWordData, m_DictMetadata.getSameTypeSequence())) {
            std::shared_ptr<WordDataItem> wordDataPtr;

            if (wordData.tryGetItem(WordDataType::PureTextMeaning, wordDataPtr) ||
                    wordData.tryGetItem(WordDataType::PureTextLocalMeaning, wordDataPtr)) {
                auto &data = wordDataPtr->getData();
                meaning = std::string(data.begin(), data.end());
                result = true;
            }
        }
    }

    return result;
}

bool DictionaryPrivate::readDictionary() {
    bool result = m_BasicDictionary.readDictionary(m_IfoFilePath);
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
            assert(fileSize == m_DictMetadata.getIndexFileSize());
        }

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
