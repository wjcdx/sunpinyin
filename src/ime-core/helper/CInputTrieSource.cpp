#include "CInputTrieSource.h"

CTrie *CInputTrieSource::m_pTrie;
CUserDict* CInputTrieSource::m_pUserDict;

const TWCHAR *
CInputTrieSource::getWstr(unsigned wid)
{
    if (wid < m_pTrie->getWordCount())
        return (*m_pTrie)[wid];
    else if (m_pUserDict)
        return (*m_pUserDict)[wid];
    else
        return NULL;
}


