#ifndef SUNPY_XHSYLLABLE_SEGMENT_H
#define SUNPY_XHSYLLABLE_SEGMENT_H

#include "pinyin/input/syllable.h"
#include "common/input/segment/syllable_seg.h"
#include "path.h"
#include "ime-core/helper/CInputTrieSource.h"
#include "common/lexicon/trie.h"

#include <map>

class CLatticeFrame;

struct LexiStateKey {
public:
	unsigned m_start;
	CSyllables m_syls;
	std::vector<unsigned> m_seg_path;

	LexiStateKey() : m_start(0) {}
	LexiStateKey(unsigned start) : m_start(start) {
		m_seg_path.push_back(m_start);
	}
	LexiStateKey(unsigned start, CSyllables syls,
			std::vector<unsigned> seg_path)
		: m_start(start), m_syls(syls), m_seg_path(seg_path)
	{}

	bool operator< (const LexiStateKey &rhs) const {
		return m_start < rhs.m_start;
	}
};

typedef std::vector<const TThreadNode *> TThreadNodeVec;
typedef std::map<LexiStateKey, TThreadNodeVec> CLexiStateMap;

struct TXhSyllableSegment : TSyllableSegment {

public:
	TXhSyllableSegment (unsigned syllable,
              unsigned start, unsigned length)
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
	_buildForSingleSyllable(CLatticeFrame &ifr,
			CLatticeFrame &jfr, TSyllable syllable, CLexiStateMap &statesMap);

	void
	_buildLexiconStates(unsigned i, unsigned j);

protected:
	unsigned m_FwdStrokeNum;
	bool m_bNumMet;
	BranchList m_TrieBranches;
};

#endif
