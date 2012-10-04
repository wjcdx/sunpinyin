#ifndef SUNPY_UNIT_DATA_H
#define SUNPY_UNIT_DATA_H

#include "stdio.h"
#include "TUnit.h"
#include "trie_type.h"

using namespace TrieThreadModel;

class CUnitData {
public:
	static void initialize(enum TrieType t) { m_type = t; }
    static TUnit encode(const char *str);
    static const char *decode(TUnit u, const char **i = NULL,
			const char **f = NULL);
private:
	static enum TrieType m_type;
};

#endif
