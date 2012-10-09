// -*- mode: c++ -*-
#ifndef __SUNPINYIN_TRIE_H__
#define __SUNPINYIN_TRIE_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <map>

#include "portability.h"
#include "TrieThreadModel.h"

using namespace TrieThreadModel;

class CTrie {
public:

    enum TrieType {
        INVALID,
        PINYIN,
        XINGHUA
    };

    friend class CTrieMaker;

    CTrie() : m_Size(0), m_mem(NULL), m_words(NULL) { }

    ~CTrie()
    { free(); }

    bool
    load(const char* fileName);

    void
    free(void);

    virtual bool
    isValid(const TThreadNode* pnode, bool allowNonComplete, unsigned csLevel = 0);

    unsigned int getRootOffset() const
    { return 3 * sizeof(unsigned int); }

    const TThreadNode*getRootNode() const
    { return (TThreadNode *) (m_mem + getRootOffset()); }

    const TThreadNode*nodeFromOffset(unsigned int offset) const
    { return (offset < getRootOffset()) ? NULL : ((TThreadNode *) (m_mem + offset)); }

    unsigned int getWordCount(void) const
    { return *(unsigned int *) m_mem; }

    unsigned int getNodeCount(void) const
    { return *(unsigned int *) (m_mem + sizeof(unsigned int)); }

    unsigned int getStringOffset(void) const
    { return *(unsigned int *) (m_mem + 2 * sizeof(unsigned int)); }

    inline const TThreadNode*transfer(const TThreadNode* pnode, unsigned s) const
    { return nodeFromOffset(pnode->transfer(s)); }

    inline const TThreadNode*transfer(unsigned s) const
    { return transfer(getRootNode(), s); }

    unsigned int
    getSymbolId(const TWCHAR* wstr);

    unsigned int
    getSymbolId(const wstring & wstr);

    const TWCHAR*operator[](unsigned int idx) const
    { return m_words[idx]; }

    int
    lengthAt(unsigned int idx) const;

    void
    print(FILE *fp) const;

protected:
    unsigned int m_Size;
    char                  *m_mem;
    TWCHAR               **m_words;

    std::map<wstring, unsigned>  m_SymbolMap;
    
    void
    print(const TThreadNode* pRoot, std::string& prefix, FILE *fp) const;
};

#endif /* __SUNPINYIN_TRIE_H__*/

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
