#include "trie.h"
#include "maker_factory.h"

CTrieMaker *MakerFactory::m_Maker;

CTrieMaker *
MakerFactory::getMaker(CTrie::TrieType t) 
{
	switch (t) {
	case CTrie::PINYIN:
		m_Maker = new CPyTrieMaker();
		break;
	case CTrie::XINGHUA:
		m_Maker = new CXhTrieMaker();
		break;
	default:
		m_Maker = new CTrieMaker();
		break;
	}
	return m_Maker;
}

