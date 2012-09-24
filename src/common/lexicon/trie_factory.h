#ifndef SUNPY_TRIE_FACTORY_H
#define SUNPY_TRIE_FACTORY_H

#include "trie_type.h"

class TrieFactory {
public:

	CTrie &getTrie(enum TrieType t) 
	{
		switch (t) {
		case PINYIN:
			return new CPyTrie();
		case XINGHUA:
			return new CXhTrie();
		}
	}


}


#endif
