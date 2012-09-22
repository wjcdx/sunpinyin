// -*- mode: c++ -*-
#ifndef __SUNPINYIN_PYTRIE_H__
#define __SUNPINYIN_PYTRIE_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "portability.h"
#include "unit.h"
#include "TWordIdInfo.h"
#include <map>

class CTrie {
public:
    friend class CTrieMaker;

    CTrie() : m_Size(0), m_mem(NULL), m_words(NULL) { }

    ~CTrie()
    { free(); }

    bool
    load(const char* fileName);

    void
    free(void);

    virtual bool
    isValid(const TNode* pnode, bool allowNonComplete, unsigned csLevel = 0);

    unsigned int getRootOffset() const
    { return 3 * sizeof(unsigned int); }

    const TNode*getRootNode() const
    { return (TNode *) (m_mem + getRootOffset()); }

    const TNode*nodeFromOffset(unsigned int offset) const
    { return (offset < getRootOffset()) ? NULL : ((TNode *) (m_mem + offset)); }

    unsigned int getWordCount(void) const
    { return *(unsigned int *) m_mem; }

    unsigned int getNodeCount(void) const
    { return *(unsigned int *) (m_mem + sizeof(unsigned int)); }

    unsigned int getStringOffset(void) const
    { return *(unsigned int *) (m_mem + 2 * sizeof(unsigned int)); }

    inline const TNode*transfer(const TNode* pnode, unsigned s) const
    { return nodeFromOffset(pnode->transfer(s)); }

    inline const TNode*transfer(unsigned s) const
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

    CUnitData m_UnitData;

    void
    print(const TNode* pRoot, std::string& prefix, FILE *fp) const;
};

#endif /* __SUNPINYIN_PYTRIE_H__*/

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
