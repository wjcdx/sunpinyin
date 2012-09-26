#ifndef SUNPY_SYLLABLE_SEGMENT_H
#define SUNPY_SYLLABLE_SEGMENT_H

#include "path.h"

struct TXhSyllableSegment : TSegment {

protected:
	unsigned m_FwdStrokeNum = 1;
	bool m_NumMet = false;
	BranchVec m_TrieBranches;
}

#endif
