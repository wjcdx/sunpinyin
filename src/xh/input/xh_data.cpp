#include "xh_data.h"

XhKeyMapPair
CXhData::xhDataMaps[] = {
	{ 'a', "P01" },
};

static XhKeyMapPair &
findPair(std::string name)
{
	XhKeyMapPair *p;
	return *p;
}

static XhKeyMapPair &
findPair(int key)
{
	XhKeyMapPair *p;
	return *p;
}

TUnit
CXhData::encode(const char *name)
{
	return TUnit(findPair(name).getKey());
}

const char *
CXhData::decode(TUnit u, const char **i, const char **f)
{
	return findPair(u).getName().c_str();
}

