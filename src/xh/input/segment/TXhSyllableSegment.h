#ifndef SUNPY_XHSYLLABLE_SEGMENT_H
#define SUNPY_XHSYLLABLE_SEGMENT_H

#include "pinyin/input/syllable.h"
#include "common/input/segment/syllable_seg.h"
#include "path.h"
#include "ime-core/helper/CInputTrieSource.h"
#include "common/lexicon/trie.h"

struct TXhSyllableSegment : TSyllableSegment {

	TXhSyllableSegment (unsigned syllable,
              unsigned start,
              unsigned length)
        : TSyllableSegment(syllable, start, length),
		m_FwdStrokeNum(1), m_bNumMet(false)
    {
	}
	
	void
	forward(unsigned i, unsigned j);

private:
	void
	prepare();

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
	bool m_bNumMet;
	BranchList m_TrieBranches;
};

#endif
