#ifndef SUNPY_MAKER_FACTORY_H
#define SUNPY_MAKER_FACTORY_H

#include "trie.h"
#include "pinyin/lexicon/pytrie_maker.h"
#include "xh/lexicon/xh/CXhTrieMaker.h"

class MakerFactory {
public:
	static CTrieMaker *getMaker(CTrie::TrieType t);
private:
	static CTrieMaker *m_Maker;
};


#endif
