#include <assert.h>
#include <string>
#include "xh_data.h"

XhKeyMapPair
CXhData::xhDataMaps[] = {
	XhKeyMapPair('f', "P001"),
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
	XhKeyMapPair('v', "P004"),
	XhKeyMapPair('v', "P005"),
	XhKeyMapPair('v', "P019"),
	XhKeyMapPair('v', "P020"),
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
	XhKeyMapPair('s', "P401"),
	XhKeyMapPair('s', "P103"),
	XhKeyMapPair('s', "P105"),
	XhKeyMapPair('s', "P118"),
	XhKeyMapPair('z', "P106"),
	XhKeyMapPair('z', "P108"),
	XhKeyMapPair('z', "P120"),
	XhKeyMapPair('z', "P109"),
	XhKeyMapPair('z', "P111"),
	XhKeyMapPair('z', "P115"),
	XhKeyMapPair('z', "P117"),
	XhKeyMapPair('z', "P122"),
	XhKeyMapPair('z', "P124"),
	XhKeyMapPair('z', "P128"),
	XhKeyMapPair('z', "P130"),
	XhKeyMapPair('d', "P201"),
	XhKeyMapPair('e', "P306"),
	XhKeyMapPair('c', "P307"),
	XhKeyMapPair('x', "P301"),
	XhKeyMapPair('x', "P302"),
	XhKeyMapPair('x', "P303"),
	XhKeyMapPair('x', "P308"),
	XhKeyMapPair('x', "P304"),
	XhKeyMapPair('x', "P305"),
	XhKeyMapPair('x', "P309"),

	XhKeyMapPair('g', "S01", true),
	XhKeyMapPair('h', "S03", true),
	XhKeyMapPair('t', "S04", true),
	XhKeyMapPair('y', "S05"),
	XhKeyMapPair('u', "S06", true),
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

