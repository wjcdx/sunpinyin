#ifndef SUNPY_MAKER_FACTORY_H
#define SUNPY_MAKER_FACTORY_H

#include "trie_type.h"

class MakerFactory {
public:
	CTrieMaker &getMaker(enum TrieType t) 
	{
		switch (t) {
		case PINYIN:
			return new CTriePyMaker();
		case XINGHUA:
			return new CTrieXhMaker();
		}
	}


}


#endif
