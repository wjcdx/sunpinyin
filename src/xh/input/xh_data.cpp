#include <assert.h>
#include <string>
#include "xh_data.h"

XhKeyMapPair
CXhData::xhDataMaps[] = {
	XhKeyMapPair('f', "P01"),
	XhKeyMapPair('f', "P02"),
	XhKeyMapPair('r', "P04"),
	XhKeyMapPair('v', "P07"),
	XhKeyMapPair('m', "P06"),
	XhKeyMapPair('m', "P03"),
	XhKeyMapPair('m', "P05"),
	XhKeyMapPair('m', "P08"),
	XhKeyMapPair('m', "P09"),
	XhKeyMapPair('a', "P11"),
	XhKeyMapPair('a', "P12"),
	XhKeyMapPair('q', "P15"),
	XhKeyMapPair('s', "P13"),
	XhKeyMapPair('z', "P16"),
	XhKeyMapPair('z', "P14"),
	XhKeyMapPair('z', "P17"),
	XhKeyMapPair('z', "P18"),
	XhKeyMapPair('z', "P19"),
	XhKeyMapPair('d', "P21"),
	XhKeyMapPair('e', "P36"),
	XhKeyMapPair('c', "P37"),
	XhKeyMapPair('x', "P31"),
	XhKeyMapPair('x', "P32"),
	XhKeyMapPair('x', "P33"),
	XhKeyMapPair('x', "P38"),
	XhKeyMapPair('x', "P34"),
	XhKeyMapPair('x', "P35"),
	XhKeyMapPair('p', "P41"),
	XhKeyMapPair('g', "S01"),
	XhKeyMapPair('h', "S03"),
	XhKeyMapPair('t', "S04"),
	XhKeyMapPair('y', "S05"),
	XhKeyMapPair('u', "S06"),
	XhKeyMapPair('k', "S08"),
	XhKeyMapPair('l', "S20"),
	XhKeyMapPair('b', "S02"),
	XhKeyMapPair('j', "S12"),
	XhKeyMapPair('i', "S09"),
	XhKeyMapPair('w', "S24"),
	XhKeyMapPair('o', "S10"),
	XhKeyMapPair('n', "S11"),
	XhKeyMapPair('n', "S13"),
	XhKeyMapPair('n', "S14"),
	XhKeyMapPair('n', "S15"),
	XhKeyMapPair('n', "S16"),
	XhKeyMapPair('n', "S17"),
	XhKeyMapPair('n', "S18"),
	XhKeyMapPair('n', "S19"),
	XhKeyMapPair('n', "S21"),
	XhKeyMapPair('n', "S22"),
	XhKeyMapPair('n', "S23"),
	XhKeyMapPair('n', "S24"),
	XhKeyMapPair('n', "S25"),
	XhKeyMapPair('n', "S26"),
	XhKeyMapPair('n', "S27"),
	XhKeyMapPair('n', "S28"),
	XhKeyMapPair('n', "S29"),
	XhKeyMapPair('n', "S30"),
	XhKeyMapPair(0, ""),
};

bool
CXhData::isPattern(unsigned ch)
{
	//assert(ch >= 'a' && ch <= 'z');
	XhKeyMapPair p = findPair(ch);
	if (!p.getName().empty()
		&& p.getName().at(0) == 'P') {
		return true;
	}
	return false;
}

bool
CXhData::isStroke(unsigned ch)
{
	XhKeyMapPair p = findPair(ch);
	if (!p.getName().empty()
		&& p.getName().at(0) == 'S') {
		return true;
	}
	return false;
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

