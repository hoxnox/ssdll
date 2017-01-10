#ifndef LOOKUPDICTIONARY_H
#define LOOKUPDICTIONARY_H

#include <string>
#include <memory>
#include "ssdll_global.h"

class DictionaryPrivate;

class SSDLLSHARED_EXPORT LookupDictionary
{
public:
    LookupDictionary();
    virtual ~LookupDictionary();

public:
#ifdef _WIN32
    bool loadDictionary(const std::wstring &ifoFilepath);
#else
    bool loadDictionary(const std::string &ifoFilepath);
#endif
    void unloadDictionary();
    bool translate(const std::string &word, std::string &meaning);

private:
    std::unique_ptr<DictionaryPrivate> m_DictionaryPrivate;
};

#endif // LOOKUPDICTIONARY_H
