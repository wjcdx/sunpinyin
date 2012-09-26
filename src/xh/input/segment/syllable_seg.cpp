#include "syllable_seg.h"

void
TXhSyllableSegment::forward(unsigned i, unsigned j)
{
    std::vector<unsigned>::const_iterator it = this.m_syllables.begin();
    std::vector<unsigned>::const_iterator ite = this.m_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(*it);

	_buildLexiconStates(i, j);
}

static void
TXhSyllableSegment::_forwardSingleSyllable(TSyllable syllable)
{
	if (isdigit(syllable)) {
		_forwardNumber(syllable);
		this.m_FwdStrokeNum = syllable;
	} else {
		_forwardStroke(syllable);
		this.m_FwdStrokeNum = 1;
	}
}

void
TXhSyllableSegment::_forwardNumber(unsigned n)
{
}

void
TXhSyllableSegment::_forwardStroke(TSyllable &syllable)
{
    std::vector<TrieBranch>::iterator it = this.m_TrieBranches.begin();
    std::vector<TrieBranch>::iterator ite = this.m_TrieBranches.end();

	for (; it != ite; it++) {
		if(!it->newAdded && _forwardBranch(*it, syllable)) {
			it->erase();
		}
	}
}

bool
TXhSyllableSegment::_forwardBranch(TrieBranch &branch,
						TSyllable &syllable)
{
	bool suc = false;
	PathVec fwdPaths;

	suc = branch.getCheckPoint().forward(m_Path, syllable, fwdPaths, m_FwdStrokeNum, m_NumMeet);
	
	if (!suc)
		return false;

	if (!m_NumMeet && m_FwdStrokeNum > 1) {
		m_NumMeet = true;
	}

	PathVec::iterator it = fwdPaths.begin();
	PathVec::iterator ite = fwdPaths.end();

	for (it++; it != ite; it++) {
		TrieBranch b(branch);

		b.addPathInfo(*it);
		b.newAdded = true;
		m_TrieBranches.push_back(b);
	}
	Path &p = fwdPathes.front();
	branch.addPathInfo(p);
	return true;
}

void
TXhSyllableSegment::_buildLexiconStates(unsigned i, unsigned j, TSyllable syllable)
{
    CLatticeFrame &fr = this.getLatticeFrame(j);
    fr.m_type = CLatticeFrame::SYLLABLE;

	CSyllables syls;
	syls.push_back(syllable);
	std::vector<unsigned> seg_path;
	seg_path.push_back(this.m_start);
	seg_path.push_back(this.m_start + this.m_len);

    std::vector<TrieBranch>::iterator it = this.m_TrieBranches.begin();
    std::vector<TrieBranch>::iterator ite = this.m_TrieBranches.end();
	for (; it != ite; it++) {
		std::vector<CTrie::TNode *> &nodes = it->getTNodes();

		std::vector<CTrie::TNode>::iterator nit = nodes.begin();
		std::vector<CTrie::TNode>::iterator nite = nodes.end();
        for (; nit != nite; nit++) {
			TLexiconState new_lxst = TLexiconState(i, *nit, syls, seg_path);
			fr.m_lexiconStates.push_back(new_lxst);
		}
	}
}

