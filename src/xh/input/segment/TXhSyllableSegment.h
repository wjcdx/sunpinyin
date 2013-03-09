#ifndef SUNPY_XHSYLLABLE_SEGMENT_H
#define SUNPY_XHSYLLABLE_SEGMENT_H

#include "pinyin/input/syllable.h"
#include "common/input/segment/syllable_seg.h"
#include "path.h"
#include "ime-core/helper/CInputTrieSource.h"
#include "common/lexicon/trie.h"
//#include "xh/lattice/TXhLexiconState.h"

struct TXhLexiconState;

#include <map>

class CLatticeFrame;

struct TXhSyllableSegment : TSyllableSegment {

public:
	TXhSyllableSegment (unsigned syllable,
              unsigned start, unsigned length)
        : TSyllableSegment(syllable, start, length),
		m_FwdStrokeNum(1) 
    {
	}
	
	void
	forward(unsigned i, unsigned j);

private:
	void
	prepare(const TrieThreadModel::TThreadNode *pTNode);

	void
	_forwardFromTNode(const TrieThreadModel::TThreadNode *pTNode);

	void
	_forwardFromLastSegment(CLatticeFrame &jfr, TXhLexiconState &lxst);

	void
	_forwardFromRoot(unsigned i, CLatticeFrame &jfr);

	void
	_forwardSingleSyllable(TSyllable syllable);
	
	void
	_forwardNumber(unsigned n);
	
	void
	_forwardStroke(TSyllable &syllable);

	bool
	_forwardBranch(TrieBranch &branch, TSyllable &syllable);

protected:
	unsigned m_FwdStrokeNum;
	BranchList m_TrieBranches;
};

#endif
