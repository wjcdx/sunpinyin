#ifndef SUNPY_XHSYLLABLE_SEGMENT_H
#define SUNPY_XHSYLLABLE_SEGMENT_H

#include "pinyin/input/syllable.h"
#include "common/input/segment/syllable_seg.h"
#include "path.h"

struct TXhSyllableSegment : TSyllableSegment {

	TXhSyllableSegment (unsigned syllable,
              unsigned start,
              unsigned length)
        : TSyllableSegment(syllable, start, length),
		m_FwdStrokeNum(1), m_NumMet(false)
    {
	}
	
	void
	forward(unsigned i, unsigned j);

private:
	void
	_forwardSingleSyllable(TSyllable syllable);
	
	void
	_forwardNumber(unsigned n);
	
	void
	_forwardStroke(TSyllable &syllable);

	bool
	_forwardBranch(TrieBranch &branch, TSyllable &syllable);

	void
	_buildLexiconStates(unsigned i, unsigned j);

protected:
	unsigned m_FwdStrokeNum;
	bool m_NumMet;
	BranchList m_TrieBranches;
};

#endif
