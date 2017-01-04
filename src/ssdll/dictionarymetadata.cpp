#include "dictionarymetadata.h"
#include <fstream>
#include <sstream>

bool tryGetValue(const std::map<std::string, std::string> &keyValueStore, const std::string &key, std::string &value) {
    bool found = false;

    auto it = keyValueStore.find(key);
    if (it != keyValueStore.end()) {
        value = it->second;
        found = true;
    }

    return found;
}

bool tryGetValue(const std::map<std::string, std::string> &keyValueStore, const std::string &key, int &value) {
    bool found = false;

    auto it = keyValueStore.find(key);
    if (it != keyValueStore.end()) {
        value = std::stoi(it->second);
        found = true;
    }

    return found;
}

#ifdef _WIN32
bool DictionaryMetadata::init(const std::wstring &ifoPath, bool isTreeDict) {
    std::wifstream ifoStream(ifoPath.c_str());
#else
bool DictionaryMetadata::init(const std::string &ifoPath, bool isTreeDict) {
    std::ifstream ifoStream(ifoPath.c_str());
#endif
    std::stringstream ifoContents;
    ifoContents << ifoStream.rdbuf();

    static const char TREEDICT_MAGIC_DATA[] = "StarDict's treedict ifo file";
    static const char DICT_MAGIC_DATA[] = "StarDict's dict ifo file";
    const char *magicData = isTreeDict ? TREEDICT_MAGIC_DATA : DICT_MAGIC_DATA;
    std::string header = magicData;

    std::string currLine;
    bool anyError = false;

    std::map<std::string, std::string> options;

    do {
        ifoContents >> currLine;
        if (currLine != header) {
            anyError = true;
            break;
        }

        while (ifoContents >> currLine) {
            auto pos = currLine.find('=');
            if (pos != std::string::npos) {
                auto key = currLine.substr(0, pos);
                auto value = currLine.substr(pos + 1);
                options[key] = value;
            } else {
                anyError = true;
                break;
            }
        }
    } while (false);

    if (!anyError) {
        anyError = initFromMap(options);
    }

    bool success = !anyError;
    return success;
}

bool DictionaryMetadata::initFromMap(const std::map<std::string, std::string> &options) {
    bool anyError = false;

    do {
        // required fields
        {
            if (!tryGetValue(options, "bookname", m_Bookname)) {
                anyError = true;
                break;
            }

            if (!tryGetValue(options, "wordcount", m_WordCount)) {
                anyError = true;
                break;
            }

            if (!tryGetValue(options, "idxfilesize", m_IndexFileSize)) {
                anyError = true;
                break;
            }
        }

        tryGetValue(options, "idxoffsetbits", m_IndexOffsetBits);
        tryGetValue(options, "sametypesequence", m_SameTypeSequence);
        tryGetValue(options, "author", m_Author);
        tryGetValue(options, "email", m_Email);
        tryGetValue(options, "website", m_Website);
        tryGetValue(options, "description", m_Description);
        tryGetValue(options, "date", m_Date);
    } while (false);

    bool success = !anyError;
    return success;
}
