#include <assert.h>
#include <string>
#include "xh_data.h"

XhKeyMapPair
CXhData::xhDataMaps[] = {
	XhKeyMapPair('v', "P001"),
	XhKeyMapPair('r', "P002"),
	XhKeyMapPair('r', "P003"),
	XhKeyMapPair('r', "P006"),
	XhKeyMapPair('r', "P009"),
	XhKeyMapPair('r', "P010"),
	XhKeyMapPair('r', "P011"),
	XhKeyMapPair('r', "P012"),
	XhKeyMapPair('r', "P015"),
	XhKeyMapPair('r', "P018"),
	XhKeyMapPair('r', "P021"),
	XhKeyMapPair('r', "P024"),
	XhKeyMapPair('r', "P025"),
	XhKeyMapPair('r', "P026"),
	XhKeyMapPair('r', "P027"),
	XhKeyMapPair('r', "P030"),
	XhKeyMapPair('f', "P004"),
	XhKeyMapPair('f', "P005"),
	XhKeyMapPair('f', "P019"),
	XhKeyMapPair('f', "P020"),
	XhKeyMapPair('m', "P013"),
	XhKeyMapPair('m', "P014"),
	XhKeyMapPair('m', "P016"),
	XhKeyMapPair('m', "P017"),
	XhKeyMapPair('m', "P028"),
	XhKeyMapPair('m', "P029"),
	XhKeyMapPair('m', "P031"),
	XhKeyMapPair('m', "P032"),
	XhKeyMapPair('p', "P007"),
	XhKeyMapPair('p', "P008"),
	XhKeyMapPair('p', "P022"),
	XhKeyMapPair('p', "P023"),
	XhKeyMapPair('a', "P101"),
	XhKeyMapPair('q', "P102"),
	XhKeyMapPair('q', "P104"),
	XhKeyMapPair('q', "P107"),
	XhKeyMapPair('q', "P110"),
	XhKeyMapPair('q', "P112"),
	XhKeyMapPair('q', "P113"),
	XhKeyMapPair('q', "P114"),
	XhKeyMapPair('q', "P116"),
	XhKeyMapPair('q', "P119"),
	XhKeyMapPair('q', "P121"),
	XhKeyMapPair('q', "P123"),
	XhKeyMapPair('q', "P125"),
	XhKeyMapPair('q', "P126"),
	XhKeyMapPair('q', "P127"),
	XhKeyMapPair('q', "P129"),
	XhKeyMapPair('z', "P401"),
	XhKeyMapPair('z', "P103"),
	XhKeyMapPair('z', "P105"),
	XhKeyMapPair('z', "P118"),
	XhKeyMapPair('l', "P106"),
	XhKeyMapPair('l', "P108"),
	XhKeyMapPair('l', "P120"),
	XhKeyMapPair('l', "P109"),
	XhKeyMapPair('l', "P111"),
	XhKeyMapPair('l', "P115"),
	XhKeyMapPair('l', "P117"),
	XhKeyMapPair('l', "P122"),
	XhKeyMapPair('l', "P124"),
	XhKeyMapPair('l', "P128"),
	XhKeyMapPair('l', "P130"),
	XhKeyMapPair('d', "P201"),
	XhKeyMapPair('t', "P306"),
	XhKeyMapPair('c', "P307"),
	XhKeyMapPair('o', "P301"),
	XhKeyMapPair('o', "P302"),
	XhKeyMapPair('o', "P303"),
	XhKeyMapPair('o', "P308"),
	XhKeyMapPair('o', "P304"),
	XhKeyMapPair('o', "P305"),
	XhKeyMapPair('o', "P309"),

	XhKeyMapPair('g', "S01", true),
	XhKeyMapPair('h', "S03", true),
	XhKeyMapPair('e', "S04", true),
	XhKeyMapPair('u', "S05"),
	XhKeyMapPair('w', "S06", true),
	XhKeyMapPair('k', "S08"),
	XhKeyMapPair('n', "S20"),
	XhKeyMapPair('b', "S02"),
	XhKeyMapPair('j', "S12"),
	XhKeyMapPair('i', "S09"),
	XhKeyMapPair('x', "S24"),
	XhKeyMapPair('y', "S10"),
	XhKeyMapPair('s', "S11"),
	XhKeyMapPair('s', "S13"),
	XhKeyMapPair('s', "S14"),
	XhKeyMapPair('s', "S15"),
	XhKeyMapPair('s', "S16"),
	XhKeyMapPair('s', "S17"),
	XhKeyMapPair('s', "S18"),
	XhKeyMapPair('s', "S19"),
	XhKeyMapPair('s', "S21"),
	XhKeyMapPair('s', "S22"),
	XhKeyMapPair('s', "S23"),
	XhKeyMapPair('s', "S25"),
	XhKeyMapPair('s', "S26"),
	XhKeyMapPair('s', "S27"),
	XhKeyMapPair('s', "S28"),
	XhKeyMapPair('s', "S29"),
	XhKeyMapPair('s', "S30"),

	XhKeyMapPair('B', "B01"),
	XhKeyMapPair(0, ""),
};

bool
CXhData::isPattern(unsigned ch)
{
	//assert(ch >= 'a' && ch <= 'z');
	XhKeyMapPair &p = findPair(ch);
	if (!p.getName().empty()
		&& p.getName().at(0) == 'P') {
		return true;
	}
	return false;
}

bool
CXhData::isBoundary(unsigned ch)
{
	return (ch == 'B');
}

bool
CXhData::isSplitter(unsigned ch)
{
	return (isBoundary(ch) || isPattern(ch));
}

bool
CXhData::isCharComp(unsigned ch)
{
	return (isBoundary(ch) || isStroke(ch));
}

bool
CXhData::isStroke(unsigned ch)
{
	XhKeyMapPair &p = findPair(ch);
	if (!p.getName().empty()
		&& p.getName().at(0) == 'S') {
		return true;
	}
	return false;
}

bool
CXhData::isFirstStroke(unsigned ch)
{
	XhKeyMapPair &p = findPair(ch);
	return p.isFirst();
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

