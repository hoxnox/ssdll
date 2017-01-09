#ifndef DICTIONARYMETADATA_H
#define DICTIONARYMETADATA_H

#include <string>
#include <map>

class DictionaryMetadata
{
public:
    DictionaryMetadata():
        m_WordCount(0),
        m_IndexFileSize(0)
    { }

public:
#ifdef _WIN32
    bool init(const std::wstring &ifoPath, bool isTreeDict);
#else
    bool init(const std::string &ifoPath, bool isTreeDict);
#endif
    bool isOffset64Bit() const { return m_IndexOffsetBits == 64; }
    unsigned long long getIndexFileSize() const { return m_IndexFileSize; }
    int getWordCount() { return m_WordCount; }

private:
    bool initFromMap(const std::map<std::string, std::string> &options);

private:
    int m_WordCount;
    unsigned long long m_IndexFileSize;
    int m_IndexOffsetBits;
#ifdef _WIN32
    std::wstring m_Filepath;
    std::wstring m_BaseName;
#else
    std::string m_Filepath;
    std::string m_BaseName;
#endif
    std::string m_Bookname;
    std::string m_Author;
    std::string m_Email;
    std::string m_Website;
    std::string m_Date;
    std::string m_Description;
    std::string m_SameTypeSequence;
};

#endif // DICTIONARYMETADATA_H
