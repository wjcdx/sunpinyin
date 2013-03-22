#include <assert.h>
#include <string>
#include "xh_data.h"

XhKeyMapPair
CXhData::xhDataMaps[] = {
	XhKeyMapPair('f', "P001"),
	XhKeyMapPair('t', "P002"),
	XhKeyMapPair('t', "P003"),
	XhKeyMapPair('t', "P006"),
	XhKeyMapPair('t', "P009"),
	XhKeyMapPair('t', "P010"),
	XhKeyMapPair('t', "P011"),
	XhKeyMapPair('t', "P012"),
	XhKeyMapPair('t', "P015"),
	XhKeyMapPair('t', "P018"),
	XhKeyMapPair('t', "P021"),
	XhKeyMapPair('t', "P024"),
	XhKeyMapPair('t', "P025"),
	XhKeyMapPair('t', "P026"),
	XhKeyMapPair('t', "P027"),
	XhKeyMapPair('t', "P030"),
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
	XhKeyMapPair('x', "P007"),
	XhKeyMapPair('x', "P008"),
	XhKeyMapPair('x', "P022"),
	XhKeyMapPair('x', "P023"),
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
	XhKeyMapPair('n', "P106"),
	XhKeyMapPair('n', "P108"),
	XhKeyMapPair('n', "P120"),
	XhKeyMapPair('n', "P109"),
	XhKeyMapPair('n', "P111"),
	XhKeyMapPair('n', "P115"),
	XhKeyMapPair('n', "P117"),
	XhKeyMapPair('n', "P122"),
	XhKeyMapPair('n', "P124"),
	XhKeyMapPair('n', "P128"),
	XhKeyMapPair('n', "P130"),
	XhKeyMapPair('d', "P201"),
	XhKeyMapPair('w', "P306"),
	XhKeyMapPair('c', "P307"),
	XhKeyMapPair('y', "P301"),
	XhKeyMapPair('y', "P302"),
	XhKeyMapPair('y', "P303"),
	XhKeyMapPair('y', "P308"),
	XhKeyMapPair('y', "P304"),
	XhKeyMapPair('y', "P305"),
	XhKeyMapPair('y', "P309"),

	XhKeyMapPair('g', "S01", true),
	XhKeyMapPair('h', "S03", true),
	XhKeyMapPair('e', "S04", true),
	XhKeyMapPair('r', "S05"),
	XhKeyMapPair('u', "S06", true),
	XhKeyMapPair('k', "S08"),
	XhKeyMapPair('l', "S20"),
	XhKeyMapPair('p', "S02"),
	XhKeyMapPair('j', "S12"),
	XhKeyMapPair('i', "S09"),
	XhKeyMapPair('b', "S24"),
	XhKeyMapPair('o', "S10"),
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

