#ifndef SUNPY_CANDIDATE_H
#define SUNPY_CANDIDATE_H

#include "lattice_states.h"

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

/**
 * CCandidate represent basic information about a single candidate.
 * Its start bone and finishing bone. It's content string. and its
 * word id.
 */
class CCandidate {
    friend class CIMIContext;
    friend class CLatticeManager;
public:
    unsigned m_start;
    unsigned m_end;
    const TWCHAR       *m_cwstr;

public:
    /** Give out the constructor for convinience */
    CCandidate(unsigned start = 0,
               unsigned end = 0,
               TLexiconState* pLxst = NULL,
               const TWCHAR* s = NULL,
               unsigned int wid = 0)
        : m_start(start), m_end(end), m_cwstr(s), m_wordId(wid),
          m_pLexiconState(pLxst) {}

protected:
    unsigned int m_wordId;
    TLexiconState* m_pLexiconState;
}; // of CCandidate

typedef std::vector<CCandidate>     CCandidates;
typedef CCandidates::iterator CCandidatesIter;


#endif

