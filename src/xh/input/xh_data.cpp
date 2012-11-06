#include <string>
#include "xh_data.h"

XhKeyMapPair
CXhData::xhDataMaps[] = {
	XhKeyMapPair('a', "P01"),
	XhKeyMapPair(0, ""),
};

bool
CXhData::isPattern(unsigned ch)
{
	return true;
}

bool
CXhData::isStroke(unsigned ch)
{
	return true;
}

XhKeyMapPair &
CXhData::findPair(std::string name)
{
	XhKeyMapPair *p = xhDataMaps;
	for (; p->getKey() != 0; p++) {
		if (p->getName() == name)
			return *p;
	}
	return *p;
}

XhKeyMapPair &
CXhData::findPair(int key)
{
	XhKeyMapPair *p = xhDataMaps;
	for (; p->getKey() != 0; p++) {
		if (p->getKey() == key)
			return *p;
	}
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

