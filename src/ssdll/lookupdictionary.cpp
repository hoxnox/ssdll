#include "lookupdictionary.h"
#include "dictionaryprivate.h"

LookupDictionary::LookupDictionary()
{
    m_DictionaryPrivate.reset(new DictionaryPrivate());
}

LookupDictionary::~LookupDictionary() {
    if (m_DictionaryPrivate) {
        m_DictionaryPrivate->unloadDictionary();
    }
}

#ifdef _WIN32
bool LookupDictionary::setIfoFilePath(const std::wstring &ifoFilePath) {
#else
bool LookupDictionary::setIfoFilePath(const std::string &ifoFilePath) {
#endif
    bool success = m_DictionaryPrivate->setIfoPath(ifoFilePath);
    return success;
}

bool LookupDictionary::loadDictionary() {
    bool success = m_DictionaryPrivate->loadDictionary();
    return success;
}

bool LookupDictionary::translate(const std::string &word, std::string &translation) {
    if (!m_DictionaryPrivate) { return false; }

    bool success = m_DictionaryPrivate->translate(word, translation);
    return success;
}

bool LookupDictionary::isLoaded() const {
    if (!m_DictionaryPrivate) { return false; }

    bool result = m_DictionaryPrivate->isLoaded();
    return result;
}
