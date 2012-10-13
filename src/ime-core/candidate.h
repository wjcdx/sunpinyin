#ifndef SUNPY_CANDIDATE_H
#define SUNPY_CANDIDATE_H

#include "lattice_states.h"

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

