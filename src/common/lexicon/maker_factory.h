#ifndef SUNPY_MAKER_FACTORY_H
#define SUNPY_MAKER_FACTORY_H

#include "trie_type.h"
#include "pytrie_maker.h"
#include "CXhTrieMaker.h"

class MakerFactory {
public:
	static CTrieMaker getMaker(enum TrieType t);
};


#endif
