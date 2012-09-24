#ifndef SUNPY_UNIT_H
#define SUNPY_UNIT_H

struct TUnit {
	unsigned m_unit;
	TUnit(unsigned s) {
		m_unit = s;
	}
};

#endif
