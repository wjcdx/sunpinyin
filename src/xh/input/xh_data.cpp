#include <assert.h>
#include <stdlib.h>

#include "portability.h"
#include "xh_data.h"

using namespace TrieThreadModel;

TUnit
CXhData::encode(const char *name)
{
	return TUnit(atoi(name));
}

const char *
CXhData::decode(TUnit u, const char **i, const char **f)
{
	static char num[32];
	memset(num, 0, sizeof(num));
	sprintf(num, "%u", (unsigned)u);
	return num;
}

