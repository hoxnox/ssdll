#ifndef DICTIONARYPRIVATE_H
#define DICTIONARYPRIVATE_H

#include <memory>
#include <string>
#include <cstdio>
#include "indexfile.h"
#include "basicdictionary.h"
#include "dictziplib.hpp"
#include "dictionarymetadata.h"

class DictionaryPrivate
{
public:
#ifdef _WIN32
    DictionaryPrivate(const std::wstring &ifoFilePath);
#else
    DictionaryPrivate(const std::string &ifoFilePath);
#endif

    virtual ~DictionaryPrivate();

public:
    bool loadDictionary();
    void unloadDictionary();
    bool findPureMeaning(const std::string &word, std::string &meaning);

private:
    bool readDictionary();
    bool readIndex();

private:
#ifdef _WIN32
    std::wstring m_IfoFilePath;
#else
    std::string m_IfoFilePath;
#endif
    std::unique_ptr<IIndexFile> m_IndexFile;
    DictionaryMetadata m_DictMetadata;
    BasicDictionary m_BasicDictionary;
    volatile bool m_IsLoaded;
};

#endif // DICTIONARYPRIVATE_H
