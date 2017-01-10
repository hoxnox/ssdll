#include "lookupdictionary.h"
#include "dictionaryprivate.h"

LookupDictionary::LookupDictionary() {
}

LookupDictionary::~LookupDictionary() {
}

#ifdef _WIN32
bool LookupDictionary::loadDictionary(const std::wstring &ifoFilepath) {
#else
bool LookupDictionary::loadDictionary(const std::string &ifoFilepath) {
#endif
    m_DictionaryPrivate.reset(new DictionaryPrivate(ifoFilepath));

    bool success = m_DictionaryPrivate->loadDictionary();
    return success;
}

void LookupDictionary::unloadDictionary() {
    if (!m_DictionaryPrivate) { return; }

    m_DictionaryPrivate->unloadDictionary();
}

bool LookupDictionary::translate(const std::string &word, std::string &meaning) {
    if (!m_DictionaryPrivate) { return false; }

    bool success = m_DictionaryPrivate->findPureMeaning(word, meaning);
    return success;
}
