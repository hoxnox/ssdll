#include "dictionarybase.h"
#include <cassert>
#include <cstring>
#include "utils.h"

char* DictionaryBase::GetWordData(uint32_t idxitemOffset, uint32_t idxitemSize) {
    for (int i = 0; i < WORDDATA_CACHE_NUM; i++) {
        if (m_Cache[i].m_Data && (m_Cache[i].m_Offset == idxitemOffset)) {
            return m_Cache[i].m_Data;
        }
    }

    if (m_DictFile) {
        fseek(m_DictFile, idxitemOffset, SEEK_SET);
    }

    char *data;

    if (!m_SameTypeSequence.empty()) {
        std::vector<char> dataPlaceholder(idxitemSize, ' ');
        char *origin_data = &dataPlaceholder[0];

        if (m_DictFile) {
            const size_t nitems = fread(origin_data, idxitemSize, 1, m_DictFile);
            assert(nitems == 1);
        } else {
            m_DictDzFile->read(origin_data, idxitemOffset, idxitemSize);
        }

        uint32_t data_size;
        int sametypesequence_len = m_SameTypeSequence.length();
        //there have sametypesequence_len char being omitted.
        data_size = idxitemSize + sizeof(uint32_t) + sametypesequence_len;
        //if the last item's size is determined by the end up '\0',then +=sizeof(gchar);
        //if the last item's size is determined by the head guint32 type data,then +=sizeof(guint32);
        switch (m_SameTypeSequence[sametypesequence_len-1]) {
        case 'm':
        case 't':
        case 'y':
        case 'l':
        case 'g':
        case 'x':
        case 'k':
            data_size += sizeof(char);
            break;
        case 'W':
        case 'P':
            data_size += sizeof(uint32_t);
            break;
        default:
            if (ascii_isupper(m_SameTypeSequence[sametypesequence_len-1]))
                data_size += sizeof(uint32_t);
            else
                data_size += sizeof(char);
            break;
        }
        data = (char *)malloc(data_size);
        char *p1,*p2;
        p1 = data + sizeof(uint32_t);
        p2 = origin_data;
        uint32_t sec_size;
        //copy the head items.
        for (int i=0; i<sametypesequence_len-1; i++) {
            *p1=m_SameTypeSequence[i];
            p1+=sizeof(char);
            switch (m_SameTypeSequence[i]) {
            case 'm':
            case 't':
            case 'y':
            case 'l':
            case 'g':
            case 'x':
            case 'k':
                sec_size = strlen(p2)+1;
                memcpy(p1, p2, sec_size);
                p1+=sec_size;
                p2+=sec_size;
                break;
            case 'W':
            case 'P':
                sec_size = get_uint32(p2);
                sec_size += sizeof(uint32_t);
                memcpy(p1, p2, sec_size);
                p1+=sec_size;
                p2+=sec_size;
                break;
            default:
                if (ascii_isupper(m_SameTypeSequence[i])) {
                    sec_size = get_uint32(p2);
                    sec_size += sizeof(uint32_t);
                } else {
                    sec_size = strlen(p2)+1;
                }
                memcpy(p1, p2, sec_size);
                p1+=sec_size;
                p2+=sec_size;
                break;
            }
        }
        //calculate the last item 's size.
        sec_size = idxitemSize - (p2 - origin_data);
        *p1 = m_SameTypeSequence[sametypesequence_len-1];
        p1 += sizeof(char);

        switch (m_SameTypeSequence[sametypesequence_len-1]) {
        case 'm':
        case 't':
        case 'y':
        case 'l':
        case 'g':
        case 'x':
        case 'k':
            memcpy(p1, p2, sec_size);
            p1 += sec_size;
            *p1='\0';//add the end up '\0';
            break;
        case 'W':
        case 'P':
            set_uint32(p1, sec_size);
            p1 += sizeof(uint32_t);
            memcpy(p1, p2, sec_size);
            break;
        default:
            if (ascii_isupper(m_SameTypeSequence[sametypesequence_len-1])) {
                set_uint32(p1, sec_size);
                p1 += sizeof(uint32_t);
                memcpy(p1, p2, sec_size);
            } else {
                memcpy(p1, p2, sec_size);
                p1 += sec_size;
                *p1='\0';
            }
            break;
        }
        set_uint32(data, data_size);
    } else {
        data = (char*)malloc(idxitemSize + sizeof(uint32_t));
        if (m_DictFile) {
            const size_t nitems = fread(data+sizeof(uint32_t), idxitemSize, 1, m_DictFile);
            assert(nitems == 1);
        } else {
            m_DictDzFile->read(data+sizeof(uint32_t), idxitemOffset, idxitemSize);
        }
        set_uint32(data, idxitemSize+sizeof(uint32_t));
    }

    free(m_Cache[m_CurrCacheIndex].m_Data);

    m_Cache[m_CurrCacheIndex].m_Data = data;
    m_Cache[m_CurrCacheIndex].m_Offset = idxitemOffset;
    m_CurrCacheIndex++;

    if (m_CurrCacheIndex==WORDDATA_CACHE_NUM) {
        m_CurrCacheIndex = 0;
    }

    return data;
}

bool DictionaryBase::SearchData(std::vector<std::string> &SearchWords, uint32_t idxitemOffset, uint32_t idxitemSize, char *originData) {
    int nWord = SearchWords.size();
    std::vector<bool> WordFind(nWord, false);
    int nfound=0;

    if (m_DictFile) {
        fseek(m_DictFile, idxitemOffset, SEEK_SET);
        const size_t nitems = fread(originData, idxitemSize, 1, m_DictFile);
        assert(nitems == 1);
    } else {
        m_DictDzFile->read(originData, idxitemOffset, idxitemSize);
    }

    char *p = originData;
    uint32_t sec_size;
    int j;
    if (!m_SameTypeSequence.empty()) {
        int sametypesequence_len = m_SameTypeSequence.length();
        for (int i=0; i<sametypesequence_len-1; i++) {
            switch (m_SameTypeSequence[i]) {
            case 'm':
            case 't':
            case 'y':
            case 'l':
            case 'g':
            case 'x':
            case 'k':
                for (j=0; j<nWord; j++)
                    if (!WordFind[j] && strstr(p, SearchWords[j].c_str())) {
                        WordFind[j] = true;
                        ++nfound;
                    }


                if (nfound==nWord) {
                    return true;
                }

                sec_size = strlen(p)+1;
                p+=sec_size;
                break;
            default:
                if (ascii_isupper(m_SameTypeSequence[i])) {
                    sec_size = get_uint32(p);
                    sec_size += sizeof(guint32);
                } else {
                    sec_size = strlen(p)+1;
                }
                p+=sec_size;
            }
        }

        switch (m_SameTypeSequence[sametypesequence_len-1]) {
        case 'm':
        case 't':
        case 'y':
        case 'l':
        case 'g':
        case 'x':
        case 'k':
            sec_size = idxitemSize - (p-originData);
            for (j=0; j<nWord; j++)
                if (!WordFind[j] &&
                        strstr_len(p, sec_size, SearchWords[j].c_str())) {
                    WordFind[j] = true;
                    ++nfound;
                }


            if (nfound==nWord)
                return true;
            break;
        }
    } else {
        while (uint32_t(p - originData)<idxitemSize) {
            switch (*p) {
            case 'm':
            case 't':
            case 'y':
            case 'l':
            case 'g':
            case 'x':
            case 'k':
                for (j=0; j<nWord; j++)
                    if (!WordFind[j] && strstr(p, SearchWords[j].c_str())) {
                        WordFind[j] = true;
                        ++nfound;
                    }

                if (nfound==nWord)
                    return true;
                sec_size = strlen(p)+1;
                p+=sec_size;
                break;
            default:
                if (ascii_isupper(*p)) {
                    sec_size = get_uint32(p);
                    sec_size += sizeof(uint32_t);
                } else {
                    sec_size = strlen(p)+1;
                }
                p+=sec_size;
            }
        }
    }

    return false;
}
