#ifndef SUNPY_XHSYLLABLE_SEGMENT_H
#define SUNPY_XHSYLLABLE_SEGMENT_H

#include "syllable_seg.h"
#include "path.h"

struct TXhSyllableSegment : TSyllableSegment {

	TXhSyllableSegment (unsigned syllable,
              unsigned start,
              unsigned length)
        : TSyllableSegment(syllable, start, length), m_FwdStrokeNum(1), m_NumMet(false)
    {}

protected:
	unsigned m_FwdStrokeNum;
	bool m_NumMet;
	BranchVec m_TrieBranches;
};

#endif
