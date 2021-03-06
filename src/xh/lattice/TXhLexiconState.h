#ifndef SUNPY_XHLEXICON_STATES_H
#define SUNPY_XHLEXICON_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "ime-core/lattice/lexicon_states.h"

struct TXhLexiconState : TLexiconState {

	TXhLexiconState (unsigned start,
                   const TThreadNode *pnode,
                   CSyllables& syls,
                   std::vector<unsigned>& seg_path)
	 : TLexiconState(start, pnode, syls, seg_path)
    {}

	TXhLexiconState (unsigned start,
				   TWordIdInfoVec &words,
                   CSyllables& syls,
                   std::vector<unsigned>& seg_path)
		: TLexiconState(start, words, syls, seg_path)
	{}

    const TWordIdInfo *getWords(unsigned &num);
	const double getWeight() {
		return 1;
	}

	void setClass(unsigned n) { m_nClass = n; }
	const unsigned getClass() {
		return m_nClass;
	}

    void print(std::string prefix) const;

private:
	unsigned m_nClass;
};

#endif

