#ifndef SUNPY_TREE_TWORDINFO_H
#define SUNPY_TREE_TWORDINFO_H

namespace TrieTreeModel {

    class TWordInfo {
public:
        TWordId m_id;
        double m_cost;
        bool m_bSeen;

        TWordInfo(TWordId id = 0, double cost = 0.0, bool seen = false)
            : m_id(id), m_cost(cost), m_bSeen(seen){
            if (m_id.anony.m_bHide) {
                m_bSeen = false;
            }
            m_cost = cost + m_id.anony.m_cost;
        }

        bool operator<(const TWordInfo& b) const {
            double fa = (m_bSeen) ? (m_cost - 5000.0) : (m_cost);
            double fb = (b.m_bSeen) ? (b.m_cost - 5000.0) : (b.m_cost);
            return(fa < fb);
        }
    };
}

#endif
