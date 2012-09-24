#ifndef SUNPY_SYLLABLE_SEGMENT_H
#define SUNPY_SYLLABLE_SEGMENT_H

struct TXhSyllableSegment : TSegment {

protected:
	bool lastCharIsNum = false;
	unsigned m_CheckPointNum;
	std::vector<TrieBranch> m_TrieBranches;
}

#endif
