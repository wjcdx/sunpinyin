#include "PNodeSet.h"

bool
PNodeSet::operator<(const PNodeSet& another) const
{
    CTreeNodeSet::const_iterator t1 = m_pns->begin();
    CTreeNodeSet::const_iterator t2 = m_pns->end();
    CTreeNodeSet::const_iterator a1 = another.m_pns->begin();
    CTreeNodeSet::const_iterator a2 = another.m_pns->end();
    for (; t1 != t2 && a1 != a2; ++t1, ++a1) {
        if (*t1 < *a1) return true;
        if (*t1 > *a1) return false;
    }
    return(a1 != a2);
}

bool
PNodeSet::operator==(const PNodeSet& another) const
{
    CTreeNodeSet::const_iterator t1 = m_pns->begin();
    CTreeNodeSet::const_iterator t2 = m_pns->end();
    CTreeNodeSet::const_iterator a1 = another.m_pns->begin();
    CTreeNodeSet::const_iterator a2 = another.m_pns->end();
    for (; t1 != t2 && a1 != a2; ++t1, ++a1) {
        if (*t1 != *a1) return false;
    }
    return(a1 == a2 && t1 != t2);
}

