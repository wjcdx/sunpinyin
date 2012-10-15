#ifndef SUNPY_TCANDI_RANK_H
#define SUNPY_TCANDI_RANK_H

#include "portability.h"

/**
 * TSentenceScore is only used for whole sentence score,
 * the score from language model still using double.
 */
typedef TLongExpFloat TSentenceScore;

union TCandiRank {
public:
    bool operator<(const TCandiRank& b) const
    { return m_all < b.m_all; };

    TCandiRank() : m_all(0) {
    }

    TCandiRank(bool user, bool best, unsigned int len,
               bool fromLattice, TSentenceScore score);

    TCandiRank(bool user, bool best, unsigned int len,
               bool fromLattice, unsigned score);

protected:
    unsigned int m_all;
#if !defined(WORDS_BIGENDIAN)
    struct TAnony {
        unsigned m_cost   : 24;
        unsigned m_lattice : 1;
        unsigned m_best   : 1;
        unsigned m_len    : 5;
        unsigned m_user   : 1;
    } anony;
#else
    struct TAnony {
        unsigned m_user   : 1;
        unsigned m_len    : 5;
        unsigned m_best   : 1;
        unsigned m_lattice : 1;
        unsigned m_cost   : 24;
    } anony;
#endif
}; // TCandiRank

#endif
