#ifndef SUNPY_UNIT_DATA_H
#define SUNPY_UNIT_DATA_H

#include <stdio.h>
#include "trie.h"

class CUnitData {
public:
	static void initialize(CTrie::TrieType t);
    static TrieThreadModel::TUnit encode(const char *str);
    static const char *decode(TrieThreadModel::TUnit u, const char **i = NULL,
			const char **f = NULL);
private:
	static CTrie::TrieType m_type;
};

#endif
