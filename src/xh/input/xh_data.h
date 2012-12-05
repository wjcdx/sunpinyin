#ifndef SUNPY_XH_DATA_H
#define SUNPY_XH_DATA_H

#include "common/lexicon/thread/TUnit.h"

class CXhData {
public:
    static TrieThreadModel::TUnit encode(const char *name); 
    static const char *decode(TrieThreadModel::TUnit u, const char **i = NULL,
			const char **f = NULL);
};

#endif
