#ifndef SUNPY_IMI_DATA_H
#define SUNPY_IMI_DATA_H

#include "portability.h"

#include <stdarg.h>

#include "slm/slm.h"
#include "common/lexicon/trie.h"

void print_wide(const TWCHAR* wstr);

/**
 * Data of the IM implementation shared by all Desktop/Session(IC)
 */
class CIMIData {
public:
    CIMIData()
        : m_pTrie(NULL), m_pTrieOc(NULL), m_pLM(NULL) { }

    ~CIMIData()
    { clear(); }

    CThreadSlm*getSlm()
    { return m_pLM; }

    CTrie*getTrie()
    { return m_pTrie; }
    
    CTrie*getTrieOc()
    { return m_pTrieOc; }

    bool
    loadResource(const char* lm_file_path, const char* trie_file_path,
            const char* trie_oc);

    void
    clear();

public:
    CTrie     *m_pTrie;
    CTrie     *m_pTrieOc;
    CThreadSlm      *m_pLM;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
