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
	//used in lexicon states
	//and lattice states
	static CTrie* m_pTrie;
	//used before lexicon states
	//find syllables to be transfered
	//in m_pTrie
	static CTrie* m_pTrieOc;
    static CUserDict* m_pUserDict;
};

#endif
