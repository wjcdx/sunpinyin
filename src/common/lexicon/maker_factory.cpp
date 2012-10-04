#include "maker_factory.h"

CTrieMaker
MakerFactory::getMaker(enum TrieType t) 
{
	switch (t) {
	case PINYIN:
		return CPyTrieMaker();
	case XINGHUA:
		return CXhTrieMaker();
	}
	return CTrieMaker();
}

