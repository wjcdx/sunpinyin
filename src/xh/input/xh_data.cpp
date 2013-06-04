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
	XhKeyMapPair('a', "P004"),
	XhKeyMapPair('a', "P005"),
	XhKeyMapPair('a', "P019"),
	XhKeyMapPair('a', "P020"),
	XhKeyMapPair('y', "P013"),
	XhKeyMapPair('y', "P014"),
	XhKeyMapPair('y', "P016"),
	XhKeyMapPair('y', "P017"),
	XhKeyMapPair('y', "P028"),
	XhKeyMapPair('y', "P029"),
	XhKeyMapPair('y', "P031"),
	XhKeyMapPair('y', "P032"),
	XhKeyMapPair('x', "P007"),
	XhKeyMapPair('x', "P008"),
	XhKeyMapPair('x', "P022"),
	XhKeyMapPair('x', "P023"),
	XhKeyMapPair('n', "P101"),
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
	XhKeyMapPair('v', "P401"),
	XhKeyMapPair('v', "P103"),
	XhKeyMapPair('v', "P105"),
	XhKeyMapPair('v', "P118"),
	XhKeyMapPair('v', "P106"),
	XhKeyMapPair('v', "P108"),
	XhKeyMapPair('v', "P120"),
	XhKeyMapPair('v', "P109"),
	XhKeyMapPair('v', "P111"),
	XhKeyMapPair('v', "P115"),
	XhKeyMapPair('v', "P117"),
	XhKeyMapPair('v', "P122"),
	XhKeyMapPair('v', "P124"),
	XhKeyMapPair('v', "P128"),
	XhKeyMapPair('v', "P130"),
	XhKeyMapPair('d', "P201"),
	XhKeyMapPair('w', "P306"),
	XhKeyMapPair('c', "P307"),
	XhKeyMapPair('m', "P301"),
	XhKeyMapPair('m', "P302"),
	XhKeyMapPair('m', "P303"),
	XhKeyMapPair('m', "P308"),
	XhKeyMapPair('m', "P304"),
	XhKeyMapPair('m', "P305"),
	XhKeyMapPair('m', "P309"),

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
	XhKeyMapPair('z', "S24"),
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
	XhKeyMapPair('b', "S31", true),

	XhKeyMapPair('L', "B00"),
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
	return (ch == 'B' || ch == 'L');
}

bool
CXhData::isLastBoundary(unsigned ch)
{
	return (ch == 'L');
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

