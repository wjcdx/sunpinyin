#ifndef SUNPY_IMI_DATA_H
#define SUNPY_IMI_DATA_H

#include "portability.h"

#include <stdarg.h>

#include "slm/slm.h"
#include "trie.h"

void print_wide(const TWCHAR* wstr);

/**
 * Data of the IM implementation shared by all Desktop/Session(IC)
 */
class CIMIData {
public:
    CIMIData()
        : m_pTrie(NULL), m_pLM(NULL) { }

    ~CIMIData()
    { clear(); }

    CThreadSlm*getSlm()
    { return m_pLM; }

    CTrie*getTrie()
    { return m_pTrie; }

    bool
    loadResource(const char* lm_file_path, const char* trie_file_path);

    void
    clear();

public:
    CTrie     *m_pTrie;
    CThreadSlm      *m_pLM;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
