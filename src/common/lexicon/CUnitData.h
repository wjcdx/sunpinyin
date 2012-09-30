#ifndef SUNPY_XH_DATA_H
#define SUNPY_XH_DATA_H

#include "TUnit.h"
using namespace TrieThreadModel;

class CUnitData {

    TUnit encode(const char *); 
    const char *decode(TUnit, const char **i = NULL,
			const char **f = NULL);	

}

#endif
