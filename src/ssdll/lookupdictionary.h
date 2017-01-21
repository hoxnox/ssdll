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
    bool setIfoFilePath(const std::wstring &ifoFilePath);
#else
    bool setIfoFilePath(const std::string &ifoFilePath);
#endif
    bool loadDictionary();
    bool translate(const std::string &word, std::string &translation);
    bool isLoaded() const;

private:
    std::unique_ptr<DictionaryPrivate> m_DictionaryPrivate;
};

#endif // LOOKUPDICTIONARY_H
