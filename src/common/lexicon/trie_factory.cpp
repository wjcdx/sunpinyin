#include "trie_factory.h"

CTrie
TrieFactory::getTrie(enum TrieType t) 
{
	switch (t) {
	case PINYIN:
		//return CPyTrie();
	case XINGHUA:
		//return CXhTrie();
		break;
	}
	return CTrie();
}
