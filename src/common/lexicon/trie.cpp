#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <deque>

#include "trie.h"

using TrieThreadModel;

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

bool
CTrie::isValid(const TThreadNode* pnode,
                     bool allowNonComplete,
                     unsigned csLevel)
{
    if ((pnode != NULL) && (csLevel <= pnode->m_csLevel))
        return allowNonComplete;
    return false;
}

int
CTrie::lengthAt(unsigned int idx) const
{
    if (idx < getWordCount() - 1) {
        return (m_words[idx + 1] - m_words[idx]) - 1;
    } else if (idx == getWordCount() - 1) {
        return (((TWCHAR*)(m_mem + m_Size)) - m_words[idx]) - 1;
    }
    return 0;
}

unsigned int
CTrie::getSymbolId(const TWCHAR* wstr)
{
    std::map<wstring, unsigned>::const_iterator it;

    it = m_SymbolMap.find(wstring(wstr));
    if (it != m_SymbolMap.end())
        return it->second;
    return 0;
}

unsigned int
CTrie::getSymbolId(const wstring & wstr)
{
    std::map<wstring, unsigned>::const_iterator it;

    it = m_SymbolMap.find(wstr);
    if (it != m_SymbolMap.end())
        return it->second;
    return 0;
}

void
CTrie::free(void)
{
    if (m_mem) {
#ifdef HAVE_SYS_MMAN_H
        munmap(m_mem, m_Size);
#else
        delete [] m_mem;
#endif
        m_mem = NULL;
    }
    if (m_words) {
        delete [] m_words;
        m_words = NULL;
    }
    m_SymbolMap.clear();
}

bool
CTrie::load(const char *fname)
{
    free();

    bool suc = false;
    int fd = open(fname, O_RDONLY);
    if (fd == -1) return false;

    m_Size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

#ifdef HAVE_SYS_MMAN_H
    suc =
        (m_mem =
             (char*)mmap(NULL, m_Size, PROT_READ, MAP_SHARED, fd,
                         0)) != MAP_FAILED;
#else
    suc = (m_mem = new char [m_Size]) != NULL;
    suc = suc && (read(fd, m_mem, m_Size) > 0);
#endif
    close(fd);

    suc = suc && ((m_words = new TWCHAR*[getWordCount()]) != NULL);

    if (suc) {
        TWCHAR *p = (TWCHAR*)(m_mem + getStringOffset());
        for (int i = 0, sz = getWordCount(); i < sz; ++i) {
            m_words[i] = p;
            while (*p++)
                ;
        }
        for (unsigned i = 1; i < 100; ++i) {
            if (*m_words[i] != WCH_NULL && *m_words[i] != WCH_LESSTHAN)
                m_SymbolMap[wstring(m_words[i])] = i;
        }
    }
    return suc;
}

void
CTrie::print(FILE *fp) const
{
    std::string prefix;
    print(getRootNode(), prefix, fp);
}

void
CTrie::print(const TThreadNode* pRoot, std::string& prefix, FILE *fp) const
{
    static char buf[1024];
    if (pRoot->m_nWordId > 0) {
        fprintf(fp, "%s", prefix.c_str());
        if (pRoot->m_csLevel)
            fprintf(fp, "(GBK+)");
        unsigned int sz = pRoot->m_nWordId;
        const TWordIdInfo *pwids = pRoot->getWordIdPtr();
        for (unsigned int i = 0; i < sz; ++i) {
            unsigned int id = pwids[i].m_id;
            const TWCHAR *pw = operator[](id);
            int len = WCSLEN(pw);
            if (len != lengthAt(id)) {
                printf(" (lengthAt %d error) ", id);
            }
            WCSTOMBS(buf, pw, 1024);
            fprintf(fp, " %s", buf);
            if (pwids[i].m_bSeen == 0)
                fprintf(fp, "[x]");
            else
                fprintf(fp, "[o]");

            fprintf(fp, "(%d)", pwids[i].m_cost);
        }
        fprintf(fp, "\n");
    }
    unsigned int sz = pRoot->m_nTransfer;
    const TTransUnit* ptrans = pRoot->getTrans();
    for (unsigned int i = 0; i < sz; ++i) {
        unsigned u = ptrans[i].m_Unit;
        const TThreadNode *pch = transfer(pRoot, u);
        const char *str = m_UnitData::decode(u);
        if (!str) break;
        prefix = prefix + str + '\'';
        print(pch, prefix, fp);
        prefix.resize(prefix.size() - strlen(str) - 1);
    }
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
