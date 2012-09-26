#ifndef SUNPY_SYLLABLE_SEGMENT_H
#define SUNPY_SYLLABLE_SEGMENT_H

struct TXhSyllableSegment : TSegment {

protected:
	unsigned m_FwdStrokeNum = 1;
	bool m_NumMeet = false;
	std::vector<TrieBranch> m_TrieBranches;
}

#endif
