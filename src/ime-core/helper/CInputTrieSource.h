#ifndef SUNPY_INPUT_TRIE_SOURCE_H
#define SUNPY_INPUT_TRIE_SOURCE_H

#include "userdict.h"
#include "trie.h"

class CInputTrieSource {
public:
    void setUserDict(CUserDict *pUserDict) { m_pUserDict = pUserDict; }
    CUserDict* getUserDict() { return m_pUserDict; }
	
	static const TWCHAR * getWstr(unsigned wid);

public:
	static CTrie* m_pTrie;
    static CUserDict* m_pUserDict;
};

#endif
