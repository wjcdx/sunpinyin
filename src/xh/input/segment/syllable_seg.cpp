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

	suc = branch.forward(syllable, m_FwdStrokeNum, m_NumMet, fwdPaths);
	
	if (!suc)
		return false;

	if (!m_NumMet && m_FwdStrokeNum > 1) {
		m_NumMet = true;
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

    BranchVec::iterator it = this.m_TrieBranches.begin();
    BranchVec::iterator ite = this.m_TrieBranches.end();
	for (; it != ite; it++) {
		PathNodeVec &nodes = (*it).getPathNodes();

		PathNodeVec::iterator nit = nodes.begin();
		PathNodeVec::iterator nite = nodes.end();
        for (; nit != nite; nit++) {
			TLexiconState new_lxst = TLexiconState(i, (*nit).getTNode(), syls, seg_path);
			fr.m_lexiconStates.push_back(new_lxst);
		}
	}
}

