#include "dictionaryprivate.h"
#include <cassert>
#include <cstring>
#include "utils.h"

bool startsWith(const std::vector<char> &data, size_t pos, const std::string &prefix) {
    bool anyError = false;

    const size_t size = data.size();
    const size_t prefixSize = prefix.size();
    size_t i = pos, j = 0;

    while ((i < size) && (j < prefixSize)) {
        if (data[i] != prefix[j]) {
            anyError = true;
            break;
        }

        i++;
        j++;
    }

    bool ok = (!anyError) && (j == prefixSize) && (i <= size);
    return ok;
}

bool tryGetTag(const std::vector<char> &data, size_t pos, std::string &tagName) {
    if (pos >= data.size()) { return false; }
    assert(data[pos] == '<');
    if (data[pos] != '<') { return false; }

    const size_t maxTagLen = 10;
    tagName.clear();
    tagName.reserve(maxTagLen);

    size_t size = data.size(), i = pos + 1;
    while (i < size) {
        char c = data[i];
        if ((c != '>') && ((i - pos) <= maxTagLen)) {
            tagName.push_back(c);
            i++;
        } else {
            break;
        }
    }

    bool success = (i < size) && (data[i] == '>') && (!tagName.empty());
    return success;
}

std::string simplifyMarkup(const std::vector<char> &xdxfLikeData) {
#define GREATER "&gt;"
#define LESS "&lt;"
#define AMPERSAND "&amp;"
#define QUOTATION "&quot;"
#define APOSTROPHE "&apos;"

    std::vector<char> result;
    size_t size = xdxfLikeData.size(), i = 0;
    result.reserve(size);
    std::string tag;

    while (i < size) {
        char c = xdxfLikeData[i];

        if (c == '&') {
            if (startsWith(xdxfLikeData, i, GREATER)) {
                result.push_back('>');
                i += strlen(GREATER);
            } else if (startsWith(xdxfLikeData, i, LESS)) {
                result.push_back('<');
                i += strlen(LESS);
            } else if (startsWith(xdxfLikeData, i, AMPERSAND)) {
                result.push_back('&');
                i += strlen(AMPERSAND);
            } else if (startsWith(xdxfLikeData, i, QUOTATION)) {
                result.push_back('\"');
                i += strlen(QUOTATION);
            } else if (startsWith(xdxfLikeData, i, APOSTROPHE)) {
                result.push_back('\'');
                i += strlen(APOSTROPHE);
            } else {
                result.push_back(c);
                i++;
            }
        } else if ((c == '<') && tryGetTag(xdxfLikeData, i, tag)) {
            bool skip = false;

            if ((tag == "b") || (tag == "s") || (tag == "i")) {
                result.push_back(c);
                i++;
                skip = true;
            }

            else if (tag == "tr") {
                result.push_back('[');
            } else if (tag == "/tr") {
                result.push_back(']');
            }

            else if (tag == "kref") {
                result.push_back('{');
            } else if (tag == "/kref") {
                result.push_back('}');
            }

            else if ((tag == "ex") || (tag == "/ex")) {
                result.push_back('\\');
            }

            if (!skip) {
                i += tag.size() + 1 + 1;
            }
        } else {
            result.push_back(c);
            i++;
        }
    }

#undef GREATER
#undef LESS
#undef AMPERSAND
#undef QUOTATION
#undef APOSTROPHE
    std::string strResult(result.begin(), result.end());
    return strResult;
}

DictionaryPrivate::DictionaryPrivate():
    m_IsLoaded(false)
{
}

DictionaryPrivate::~DictionaryPrivate() {
    if (m_IsLoaded) {
        unloadDictionary();
    }
}

#ifdef _WIN32
bool DictionaryPrivate::setIfoPath(const std::wstring &ifoFilePath) {
#else
bool DictionaryPrivate::setIfoPath(const std::string &ifoFilePath) {
#endif
    if (m_IsLoaded) { return false; }

    m_IfoFilePath = ifoFilePath;
    return true;
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
    // m_IfoFilePath.clear();
    m_DictMetadata.clear();
    m_IndexFile.reset();
    m_BasicDictionary.unload();
    m_IsLoaded = false;
}

bool DictionaryPrivate::translate(const std::string &word, std::string &translation) {
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
                translation = std::string(data.begin(), data.end());
                result = true;
            } else if (wordData.tryGetItem(WordDataType::XdxfMarkupData, wordDataPtr) ||
                       wordData.tryGetItem(WordDataType::PangoMarkupData, wordDataPtr)) {
                auto &data = wordDataPtr->getData();
                translation = simplifyMarkup(data);
                result = true;
            } else if (wordData.tryGetItem(WordDataType::PowerWordData, wordDataPtr) ||
                       wordData.tryGetItem(WordDataType::KanaData, wordDataPtr)) {
                auto &data = wordDataPtr->getData();
                translation = std::string(data.begin(), data.end());
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
