#ifndef SUNPY_TRIE_FACTORY_H
#define SUNPY_TRIE_FACTORY_H

#include "trie.h"

class TrieFactory {
public:
	static CTrie *getTrie(CTrie::TrieType t);
private:
	static CTrie *m_Trie;
};

#endif
