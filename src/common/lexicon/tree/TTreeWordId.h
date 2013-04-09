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
			// 17bits should be enough for word id
            unsigned m_id       : 17;
            unsigned m_foreign	: 1;
            unsigned m_nStkNum  : 6;
        #else
            unsigned m_nStkNum  : 6;
            unsigned m_foreign	: 1;
            unsigned m_id       : 17;
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
			anony.m_foreign = 0;
        }

		TTreeWordId& operator=(const TTreeWordId &rhs) {

			anony.m_nStkNum = rhs.anony.m_nStkNum;
			anony.m_foreign = rhs.anony.m_foreign;
			anony.m_id = rhs.anony.m_id;
			anony.m_csLevel = rhs.anony.m_csLevel;
			anony.m_cost = rhs.anony.m_cost;
			anony.m_bHide = rhs.anony.m_bHide;

			return *this;
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

		void setForeign() { anony.m_foreign = 1; }
		bool isForeign() const
		{ return (anony.m_foreign == 1); }
    };

	typedef std::set<TTreeWordId> CTreeWordSet;
}

#endif
