#ifndef SUNPY_TREE_TWORDID_H
#define SUNPY_TREE_TWORDID_H

#include <set>

namespace TrieTreeModel {
    union TTreeWordId {
        unsigned int m_all;
        struct TAnony {                     //Some compiler do not support anonymous defaultly
        #ifdef WORDS_BIGENDIAN
            unsigned m_bHide    : 1;
            unsigned m_cost     : 5;
            unsigned m_csLevel  : 2;
			// 18bits should be enough for word id
            //unsigned m_id       : WORD_ID_WIDTH;
            unsigned m_id       : 18;
            unsigned m_nStkNum  : 6;
        #else
            unsigned m_nStkNum  : 6;
            unsigned m_id       : 18;
            //unsigned m_id       : WORD_ID_WIDTH;
            unsigned m_csLevel  : 2;
            unsigned m_cost     : 5;
            unsigned m_bHide    : 1;
        #endif
        } anony;

public:
        TTreeWordId() : m_all(0) {
        }
        TTreeWordId(const TTreeWordId &b) : m_all(b.m_all) {
        }
        TTreeWordId(unsigned id,
                unsigned cost = 0,
                unsigned hide = 0,
                unsigned cslvl = 0){
            anony.m_id = id;
            anony.m_cost = cost<31? cost: 31;
            anony.m_bHide = (hide) ? 1 : 0;
            anony.m_csLevel = cslvl<3? cslvl: 3;
        }

        bool operator<(const TTreeWordId& b) const
        {
			if (anony.m_nStkNum == b.anony.m_nStkNum) {
				return anony.m_id < b.anony.m_id;
			} else {
				return anony.m_nStkNum < b.anony.m_nStkNum;
			}
		}

        bool operator==(const TTreeWordId& b) const
        { return anony.m_id == b.anony.m_id; }

        operator unsigned int() const
        { return anony.m_id; }

		void setStrokeNumber(unsigned num) { anony.m_nStkNum = num; }
		unsigned getStrokeNumber() const { return anony.m_nStkNum; }
    };

	typedef std::set<TTreeWordId> CTreeWordSet;
}

#endif
