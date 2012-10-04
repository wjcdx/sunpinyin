#ifndef SUNPY_TUNIT_INFO_H
#define SUNPY_TUNIT_INFO_H

struct TUnitInfo {
    std::string m_ustr;
    int m_cost;

    TUnitInfo(const char* ustr = NULL, int cost = 0) : m_ustr(ustr), m_cost(cost)
    {
    }
    bool
    operator<(const TUnitInfo& b) const
    {
        return m_ustr < b.m_ustr;
    }
};

#endif
