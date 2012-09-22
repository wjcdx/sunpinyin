#ifndef SUNPY_TWORDIDINFO_H
#define SUNPY_TWORDIDINFO_H

#define WORD_ID_WIDTH       24

namespace TrieThreadModel {

	struct TWordIdInfo {
#ifdef WORDS_BIGENDIAN
		unsigned m_bSeen    : 1;
		unsigned m_cost     : 5;
		unsigned m_csLevel  : 2;
		unsigned m_id       : WORD_ID_WIDTH;
#else
		unsigned m_id       : WORD_ID_WIDTH;
		unsigned m_csLevel  : 2;
		unsigned m_cost     : 5;
		unsigned m_bSeen    : 1;
#endif

		TWordIdInfo() { memset(this, 0, sizeof(TWordIdInfo)); }

		TWordIdInfo(unsigned id,
					unsigned len = 0,
					unsigned seen = 0,
					unsigned cost = 0,
					unsigned cslvl = 0)
			: m_id(id), m_csLevel(cslvl), m_cost(cost), m_bSeen(seen) { }

		operator unsigned int() const { return m_id; }
	};
}

#endif
