#ifndef SUNPY_TUNIT_H
#define SUNPY_TUNIT_H

namespace TrieThreadModel {
	struct TUnit {
		unsigned m_Unit;

		TUnit() {}
		TUnit(unsigned u) {
			m_Unit = u;
		}

		operator unsigned int() const
		{ return *((unsigned *) this); }

		bool operator ==(const TUnit & unit) const {
			return m_Unit == unit.m_Unit;
		}

		bool operator <(const TUnit & unit) const {
			return m_Unit == unit.m_Unit;
		}

		bool operator ==(const unsigned s) const {
			return (*(unsigned *)this) == s;
		}

		bool operator <(const unsigned s) const {
			return (*(unsigned *)this) < s;
		}
	};
}

#endif
