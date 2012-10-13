#include "TCandiRank.h"

TCandiRank::TCandiRank(bool user, bool best, unsigned len,
                       bool fromLattice, TSentenceScore score)
{
    anony.m_user = (user) ? 0 : 1;
    anony.m_best = (best) ? 0 : 1;
    anony.m_len = (len > 31) ? (0) : (31 - len);
    anony.m_lattice = (fromLattice) ? 0 : 1;

    double ds = -score.log2();

    //make it 24-bit
    if (ds > 32767.0)
        ds = 32767.0;
    else if (ds < -32768.0)
        ds = -32768.0;
    unsigned cost = unsigned((ds + 32768.0) * 256.0);
    anony.m_cost = cost;
}

TCandiRank::TCandiRank(bool user, bool best, unsigned len,
                       bool fromLattice, unsigned rank)
{
    anony.m_user = (user) ? 0 : 1;
    anony.m_best = (best) ? 0 : 1;
    anony.m_len = (len > 31) ? (0) : (31 - len);
    anony.m_lattice = (fromLattice) ? 0 : 1;
    anony.m_cost = rank;
}

