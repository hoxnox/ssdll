#ifndef WORDDATA_H
#define WORDDATA_H

#include <vector>
#include <map>
#include <string>

enum class WordDataType {
    // data types from StarDict format specs
    PureTextMeaning, //m
    PureTextLocalMeaning, //l
    PangoMarkupData, //g
    PhoneticString, //t
    XdxfMarkupData, //x
    KanaData, //y
    PowerWordData, //k
    MediaWikiData, //w
    HtmlData, //h
    ResouceList, //r
    WavFile, //W
    PictureFile, //P
    ExperimentalData, //X
    Unknown
};

class WordDataItem
{
public:
    WordDataItem(): m_DataType(WordDataType::Unknown) {}
    WordDataItem(std::vector<char> &&data, WordDataType dataType):
        m_Data(data),
        m_DataType(dataType)
    {}

    WordDataItem(WordDataItem &&other):
        m_Data(std::move(other.m_Data)),
        m_DataType(other.m_DataType)
    {}

public:
    const std::vector<char> &getData() const { return m_Data; }
    WordDataType getType() const { return m_DataType; }

private:
    std::vector<char> m_Data;
    WordDataType m_DataType;
};

class WordData {
public:
    WordData() {}

public:
    bool parse(const std::vector<char> &data, const std::string &sameTypeSequence);

private:
    bool parseWithSameTypeSequence(const std::vector<char> &data, const std::string &sameTypeSequence);
    bool parseWithoutSameTypeSequence(const std::vector<char> &data);
    bool addDataChunk(const std::vector<char> &data, int start, int end, char dataTypeC);

private:
    std::map<WordDataType, WordDataItem> m_DataItems;
};

#endif // WORDDATA_H
