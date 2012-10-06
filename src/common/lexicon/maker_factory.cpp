#include "maker_factory.h"
#include "stdio.h"

CTrieMaker *MakerFactory::m_Maker;

CTrieMaker *
MakerFactory::getMaker(TrieType t) 
{
	switch (t) {
	case PINYIN:
		printf("new CPyTrieMaker\n");
		m_Maker = new CPyTrieMaker();
		break;
	case XINGHUA:
		printf("new CXhTrieMaker\n");
		m_Maker = new CXhTrieMaker();
		break;
	default:
		m_Maker = new CTrieMaker();
		break;
	}
	return m_Maker;
}

