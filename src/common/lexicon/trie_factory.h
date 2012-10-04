#ifndef SUNPY_TRIE_FACTORY_H
#define SUNPY_TRIE_FACTORY_H

#include "trie_type.h"
#include "trie.h"

class TrieFactory {
public:
	static CTrie getTrie(enum TrieType t);
};

#endif
