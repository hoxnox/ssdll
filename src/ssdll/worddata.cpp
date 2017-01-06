#include "worddata.h"
#include <cassert>
#include <cstdint>

WordDataType parseDataType(char dataType) {
    switch (dataType) {
    case 'm': return WordDataType::PureTextMeaning;
    case 'l': return WordDataType::PureTextLocalMeaning;
    case 'g': return WordDataType::PangoMarkupData;
    case 't': return WordDataType::PhoneticString;
    case 'x': return WordDataType::XdxfMarkupData;
    case 'y': return WordDataType::KanaData;
    case 'k': return WordDataType::PowerWordData;
    case 'w': return WordDataType::MediaWikiData;
    case 'h': return WordDataType::HtmlData;
    case 'r': return WordDataType::ResouceList;
    case 'W': return WordDataType::WavFile;
    case 'P': return WordDataType::PictureFile;
    case 'X': return WordDataType::ExperimentalData;
    default: return WordDataType::Unknown;
    }
}

bool tryFindZeroChar(const std::vector<char> &data, size_t startWith, size_t &pos) {
    bool found = false;
    size_t len = data.size();

    for (size_t i = startWith; i < len; ++i) {
        if (data[i] == '\0') {
            pos = i;
            found = true;
            break;
        }
    }

    return found;
}

bool readUInt32(const std::vector<char> &data, size_t pos, uint32_t &data) {
    assert((0 <= pos) && (pos < data.size()));
    bool ok = false;

    size_t len = data.size();
    if (pos + 3 <= len) {
        char *arr = &data[pos];
        data = *((uint32_t*)s_num);
        ok = true;
    }

    return ok;
}

bool WordData::parse(const std::vector<char> &data, const std::string &sameTypeSequence) {
    bool parseResult = false;

    if (!data.empty()) {
        if (sameTypeSequence.empty()) {
            parseResult = parseWithoutSameTypeSequence(data);
        } else {
            parseResult = parseWithSameTypeSequence(data, sameTypeSequence);
        }
    }

    return parseResult;
}

bool WordData::parseWithSameTypeSequence(const std::vector<char> &data, const std::string &sameTypeSequence) {
    size_t pos = 0, posMax = data.size() - 1;
    size_t sequenceIndex = 0;
    bool anyError = false;

    while (pos <= posMax) {
        size_t delimPos;
        bool hasNext = tryFindZeroChar(data, pos + 1, delimPos);
        size_t endPos = hasNext ? (delimPos - 1) : posMax;

        char typeC = sameTypeSequence[sequenceIndex];
        if (addDataChunk(data, pos, endPos, typeC)) {
            sequenceIndex++;
            pos = endPos + 1;
        } else {
            anyError = true;
            break;
        }
    }

    return !anyError;
}

bool WordData::parseWithoutSameTypeSequence(const std::vector<char> &data) {
    bool success = false;

    size_t pos = 0, delimPos = 0, blockSize = 0,
            posMax = data.size() - 1;
    bool anyError = false;

    while (pos <= posMax) {
        char c = data[pos];

        switch (c) {
        case 'm':
        case 'l':
        case 'g':
        case 't':
        case 'x':
        case 'y':
        case 'k':
        case 'w':
        case 'h':
        case 'r':
            // block should be delimited by \0
            bool hasNext = tryFindZeroChar(data, pos + 1, delimPos);
            size_t endPos = hasNext ? (delimPos - 1) : posMax;
            if (!addDataChunk(data, pos + 1, endPos, c)) {
                anyError = true;
            }

            pos = endPos + 1;

            break;
        case 'W':
        case 'P':
        case 'X':
            // read uint32 size and then block
            bool added = false;
            if (readUInt32(data, pos + 1, blockSize)) {
                blockSize = ntohl(blockSize);
                size_t blockStart = pos + 1 + sizeof(uint32_t);
                size_t blockEnd = blockStart + blockSize - 1;
                assert(blockEnd <= posMax);
                blockEnd = std::min(blockEnd, posMax);

                if (addDataChunk(data, blockStart, blockEnd, c)) {
                    added = true;
                }

                pos = blockEnd + 1;
            }

            if (!added) { anyError = true; }

            break;
        default:
            anyError = true;
            break;
        }

        if (anyError) { break; }
    }

    success = !anyError;
    return success;
}

bool WordData::addDataChunk(const std::vector<char> &data, int start, int end, char dataTypeC) {
    assert(start <= end);

    auto dataType = parseDataType(dataTypeC);
    auto it = m_DataItems.find(dataType);
    bool nothingLikeThatIsAdded = (it != m_DataItems.end());
    assert(nothingLikeThatIsAdded);

    if (nothingLikeThatIsAdded) {
        m_DataItems.insert(
                    std::make_pair(dataType,
                                   WordDataItem(std::vector(data.begin() + start, data.begin() + end), dataType)));
    }
}
