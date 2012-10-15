#ifndef SUNPY_INPUT_TRIE_SOURCE_H
#define SUNPY_INPUT_TRIE_SOURCE_H

#include "ime-core/userdict.h"
#include "common/lexicon/trie.h"

class CInputTrieSource {
public:
    static void setUserDict(CUserDict *pUserDict) { m_pUserDict = pUserDict; }
    static CUserDict* getUserDict() { return m_pUserDict; }
	
	static const TWCHAR * getWstr(unsigned wid);

public:
	static CTrie* m_pTrie;
    static CUserDict* m_pUserDict;
};

#endif
