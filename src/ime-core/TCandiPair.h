#ifndef SUNPY_TCANDI_PAIR_H
#define SUNPY_TCANDI_PAIR_H

struct TCandiPair {
    CCandidate m_candi;
    TCandiRank m_Rank;

    TCandiPair() : m_candi(), m_Rank()
    {
    }
};

struct TCandiPairPtr {
    TCandiPair*                     m_Ptr;

    TCandiPairPtr(TCandiPair* p = NULL) : m_Ptr(p)
    {
    }

    bool
    operator<(const TCandiPairPtr& b) const
    {
        return m_Ptr->m_Rank < b.m_Ptr->m_Rank;
    }
};

#endif
