#include "trie_factory.h"

CTrie* TrieFactory::m_Trie;

CTrie*
TrieFactory::getTrie(TrieType t) 
{
	switch (t) {
	/*
	case PINYIN:
		m_Trie = new CPyTrie();
		break;
	case XINGHUA:
		m_Trie = new CXhTrie();
		break;
	*/
	default:
		m_Trie = new CTrie();
		break;
	}
	return m_Trie;
}
