#include "lookupdictionary.h"
#include "dictionaryprivate.h"

LookupDictionary::LookupDictionary() {
}

LookupDictionary::~LookupDictionary() {
    if (m_DictionaryPrivate) {
        m_DictionaryPrivate->unloadDictionary();
    }
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

bool LookupDictionary::translate(const std::string &word, std::string &translation) {
    if (!m_DictionaryPrivate) { return false; }

    bool success = m_DictionaryPrivate->translate(word, translation);
    return success;
}
