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
    DictionaryPrivate();
    virtual ~DictionaryPrivate();

public:
    bool isLoaded() const { return m_IsLoaded; }

public:
#ifdef _WIN32
    bool setIfoPath(const std::wstring &ifoFilePath);
#else
    bool setIfoPath(const std::string &ifoFilePath);
#endif
    bool loadDictionary();
    void unloadDictionary();
    bool translate(const std::string &word, std::string &translation);

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
