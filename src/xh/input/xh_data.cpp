
#include "xh_data.h"

struct XhKeyMapPair {
	int key;
	char *name;
} xhDataMap = {
	{ 'a', "P01" },
};

XhKeyMapPair &
XhData::findPair(const char *name)
{
}

XhKeyMapPair &
XhData::findPair(int key)
{
}

TUnit
XhData::encode(const char *name)
{
	return TUnit(findPair(name).getKey());

}

const char *
XhData::decode(TUnit u)
{
	return findPair(name).getName();
}

