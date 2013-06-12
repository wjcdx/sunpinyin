#ifndef SUNPY_INPUT_TRIE_SOURCE_H
#define SUNPY_INPUT_TRIE_SOURCE_H

#include "ime-core/userdict.h"
#include "common/lexicon/trie.h"

class CInputTrieSource {
public:
    void setUserDict(CUserDict *pUserDict) { m_pUserDict = pUserDict; }
    CUserDict* getUserDict() { return m_pUserDict; }

	void setTrie(CTrie* pTrie) { m_pTrie = pTrie; }
	CTrie* getTrie() { return m_pTrie; }
	
	const TWCHAR * getWstr(unsigned wid);

public:
	CTrie* m_pTrie;
    CUserDict* m_pUserDict;
};

#endif
