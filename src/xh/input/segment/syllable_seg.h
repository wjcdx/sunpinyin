#ifndef SUNPY_SYLLABLE_SEGMENT_H
#define SUNPY_SYLLABLE_SEGMENT_H

struct TXhSyllableSegment : TSegment {

protected:
	unsigned m_FwdStrokeNum;
	std::vector<TrieBranch> m_TrieBranches;
}

#endif
