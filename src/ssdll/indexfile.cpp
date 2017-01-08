#include "indexfile.h"
#include <cstdio>
#include "utils.h"
#include "mapfile.hpp"

static inline int stardict_strcmp(const char *s1, const char *s2) {
    const int caseResult = ascii_strcasecmp(s1, s2);
    if (caseResult == 0) {
        return strcmp(s1, s2);
    } else {
        return caseResult;
    }
}

const char *OrdinaryIndexFile::CACHE_MAGIC = "StarDict's Cache, Version: 0.1";

OrdinaryIndexFile::OrdinaryIndexFile():
    m_IndexFile(nullptr),
    m_WordCount(0),
    m_Is64BitOffset(false)
{
    memset(&m_WordEntryBuffer[0], 0, sizeof(m_WordEntryBuffer));
}

OrdinaryIndexFile::~OrdinaryIndexFile() {
    if (m_IndexFile != nullptr) {
        fclose(m_IndexFile);
    }
}

#ifdef _WIN32
bool OrdinaryIndexFile::load(const std::wstring &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize) {
#else
bool OrdinaryIndexFile::load(const std::string &filepath, bool is64bitOffset, unsigned int wordCount, size_t fileSize) {
#endif
    m_WordCount = wordCount;
    m_Is64BitOffset = is64bitOffset;

    if (!loadCache(filepath)) {
        if (createCache(filepath, fileSize)) {
            saveCache(filepath);
        } else {
            return false;
        }
    }

#ifdef _WIN32
    m_IndexFile = _wfopen(filepath.c_str(), L"rb");
#else
    m_IndexFile = fopen(filepath.c_str(), "rb");
#endif
    if (m_IndexFile != nullptr) {
        m_First.assign(0, readFirstKeyOnPage(0));
        auto lastIndex = m_WordOffset.size() - 2;
        m_Last.assign(lastIndex, readFirstKeyOnPage(lastIndex));
        m_Middle.assign(lastIndex / 2, readFirstKeyOnPage(lastIndex / 2));
        m_RealLast.assign(m_WordCount - 1, retrieveKey(m_WordCount - 1));
    }
}

bool OrdinaryIndexFile::findBounds(const std::string &word, uint64_t &offset, uint32_t &size) {
    const char *key = word.c_str();

    bool lessThanFirst = (stardict_strcmp(key, m_First.m_Key.c_str()) < 0);
    bool greaterThanLast = (stardict_strcmp(m_RealLast.m_Key.c_str(), key) < 0);
    if (lessThanFirst || greaterThanLast) {
        return false;
    }

    int index = -1;
    bool found = findThroughPages(key, index);
    if (!found) {
        found = findInPage(key, index);
    } else {
        index *= ENTR_PER_PAGE;
    }

    if (found) {
        auto key = retrieveKey(index, offset, size);
        UNUSED(key);
    }

    return found;
}

#ifdef _WIN32
bool OrdinaryIndexFile::createCache(const std::wstring &idxfilepath, size_t fileSize {
#else
bool OrdinaryIndexFile::createCache(const std::string &idxfilepath, size_t fileSize) {
#endif
    MapFile mapFile;
    if (!mapFile.open(idxfilepath, fileSize)) {
        return false;
    }

    unsigned int npages = (m_WordCount - 1) / ENTR_PER_PAGE + 2;
    m_WordOffset.resize(npages, 0);

    const char *bufferBegin = mapFile.begin();

    const char *curr = bufferBegin;
    unsigned long indexSize;
    int j = 0;
    int sizesSize = m_Is64BitOffset ? (sizeof(uint32_t) + sizeof(uint64_t)) : (2*sizeof(uint32_t));

    for (int i = 0; i < m_WordCount; ++i) {
        indexSize = strlen(curr) + 1 + sizesSize;
        if (i % ENTR_PER_PAGE == 0) {
            m_WordOffset[j] = curr - bufferBegin;
            ++j;
        }

        curr += indexSize;
    }

    m_WordOffset[j] = curr - bufferBegin;

    return true;
}

#ifdef _WIN32
bool OrdinaryIndexFile::loadCache(const std::wstring &idxfilepath) {
    std::wstring cachePath = idxfilepath + L".ifo";
    struct _stati64 sb, idxsb;
    int statResult = _wstati64(cachePath.c_str(), &sb);
    int idxStatResult = _wstati64(idxfilepath.c_str(), &idxsb);
#else
bool OrdinaryIndexFile::loadCache(const std::string &idxfilepath) {
    std::string cachePath = idxfilepath + ".ifo";
    struct stat sb, idxsb;
    int statResult = stat(cachePath.c_str(), &sb);
    int idxStatResult = stat(idxfilepath.c_str(), &idxsb);
#endif

    bool success = false;

    do {
        if ((statResult != 0) || (idxStatResult != 0)) {
            break;
        }

        if (sb.st_mtime < idxsb.st_mtime) {
            break;
        }

        MapFile mf;
        if (!mf.open(cachePath, sb.st_size)) {
            break;
        }

        auto cacheMagicLength = strlen(CACHE_MAGIC);

        if (strncmp(mf.begin(), CACHE_MAGIC, cacheMagicLength) != 0) {
            break;
        }

        memcpy(&m_WordOffset[0], mf.begin() + cacheMagicLength, m_WordOffset.size()*sizeof(m_WordOffset[0]));
        success = true;
    } while (false);

    return success;
}

#ifdef _WIN32
void OrdinaryIndexFile::saveCache(const std::wstring &idxfilepath) {
    FILE *out = _wfopen(idxfilepath.c_str(), L"wb");
#else
void OrdinaryIndexFile::saveCache(const std::string &idxfilepath) {
    FILE *out = fopen(idxfilepath.c_str(), "wb");
#endif
    if (!out) { return; }

    do {
        auto cacheMagicLength = strlen(CACHE_MAGIC);

        if (fwrite(CACHE_MAGIC, 1, cacheMagicLength, out) != cacheMagicLength) {
            break;
        }

        if (fwrite(&m_WordOffset[0], sizeof(m_WordOffset[0]), m_WordOffset.size(), out) != m_WordOffset.size()) {
            break;
        }
    } while (false);

    fclose(out);
}

void OrdinaryIndexFile::CachedPage::parseIndexData(char *data, int nent, long index) {
    m_Index = index;
    char *p = data;
    long len;

    for (int i = 0; i < nent; ++i) {
        m_Entries[i].KeyString = p;
        len = strlen(p);
        p += len + 1;

        if (m_Is64BitOffset) {
            m_Entries[i].m_Offset = ntohll(get_uint64(p));
            p += sizeof(uint64_t);
        } else {
            m_Entries[i].m_Offset = ntohl(get_uint32(p));
            p += sizeof(uint32_t);
        }

        m_Entries[i].m_Size = ntohl(get_uint32(p));
        p += sizeof(uint32_t);
    }
}

int OrdinaryIndexFile::loadPage(int index) {
    unsigned long nentr = ENTR_PER_PAGE;
    if (index == (int)(m_WordOffset.size()-2)) {
        if ((nentr = (m_WordCount % ENTR_PER_PAGE)) == 0) {
            nentr = ENTR_PER_PAGE;
        }
    }

    if (index != m_Page.m_Index) {
        m_PageData.resize(m_WordOffset[index + 1] - m_WordOffset[index]);

        fseek(m_IndexFile, m_WordOffset[index], SEEK_SET);
        const size_t nitems = fread(&page_data[0], 1, m_PageData.size(), m_IndexFile);
        assert(nitems == m_PageData.size());

        m_Page.parseIndexData(&m_PageData[0], nentr, index);
    }

    return nentr;
}

char *OrdinaryIndexFile::readFirstKeyOnPage(int pageIndex) {
    fseek(m_IndexFile, m_WordOffset[pageIndex], SEEK_SET);
    uint64_t pageSize = m_WordOffset[page_idx + 1] - m_WordOffset[page_idx];
    const size_t nitems = fread(m_WordEntryBuffer,
                                std::min(sizeof(m_WordEntryBuffer), static_cast<size_t>(pageSize)),
                                1, m_IndexFile);
    assert(nitems == 1);
    // TODO: check returned values, deal with word entry that strlen>255.
    return m_WordEntryBuffer;
}

char *OrdinaryIndexFile::retrieveFirstKeyOnPage(int pageIndex) {
    char *result = nullptr;

    if (pageIndex < m_Middle.m_Index) {
        if (pageIndex == m_First.m_Index) {
            result = m_First.m_Key.c_str();
        } else {
            result = readFirstKeyOnPage(pageIndex);
        }
    } else if (pageIndex > m_Middle.m_Index) {
        if (pageIndex == m_Last.m_Index) {
            result = m_Last.m_Key.c_str();
        } else {
            result = readFirstKeyOnPage(pageIndex);
        }
    } else {
        result = m_Middle.m_Key.c_str();
    }

    return result;
}

std::string OrdinaryIndexFile::retrieveKey(int index, uint64_t &offset, uint32_t &size) {
    loadPage(index / ENTR_PER_PAGE);

    int idxInPage = index % ENTR_PER_PAGE;
    auto &entry = m_Page.m_Entries[idxInPage];

    offset = entry.m_Offset;
    size = entry.m_Size;

    std::string key = entry.KeyString;
    return key;
}

std::string OrdinaryIndexFile::retrieveKey(int index) {
    uint64_t offset;
    uint32_t size;
    return retrieveKey(index, offset, size);
}

bool OrdinaryIndexFile::findThroughPages(const char *key, int &index) {
    size_t first = 0;
    size_t last = m_WordOffset.size() - 2;
    size_t middle = first + (last - first)/2;

    while (first <= last) {
        char *curr = retrieveFirstKeyOnPage(middle);
        int cmp = stardict_strcmp(key, curr);

        if (cmp == 0) { break; }
        else if (cmp < 0) { last = middle - 1; }
        else { first = middle + 1; }

        middle = first + (last - first)/2;
    }

    bool found = first <= last;

    if (found) { index = middle; }
    else { index = last; }

    return found;
}

bool OrdinaryIndexFile::findInPage(const char *key, int &index) {
    int nentr = loadPage(index);
    size_t first = 0;
    size_t last = nentr - 1;
    size_t middle = first + (last - first)/2;

    while (first <= last) {
        char *curr = m_Page.m_Entries[middle].KeyString;
        int cmp = stardict_strcmp(key, curr);

        if (cmp == 0) { break; }
        else if (cmp < 0) { last = middle - 1; }
        else { first = middle + 1; }

        middle = first + (last - first)/2;
    }

    bool found = first <= last;

    index *= ENTR_PER_PAGE;

    if (!found) {
        index += first;    //next
    } else {
        index += middle;
    }
}
