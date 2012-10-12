#include "syllable_seg.h"
#include "TXhLexiconState.h"
#include "TXhSyllableSegment.h"
#include "lattice.h"
#include "lattice_manager.h"

void
TXhSyllableSegment::forward(unsigned i, unsigned j)
{
    std::vector<unsigned>::const_iterator it = m_syllables.begin();
    std::vector<unsigned>::const_iterator ite = m_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(*it);

	_buildLexiconStates(i, j);
}

void
TXhSyllableSegment::_forwardSingleSyllable(TSyllable syllable)
{
	if (isdigit(syllable)) {
		_forwardNumber(syllable);
		m_FwdStrokeNum = syllable;
	} else {
		_forwardStroke(syllable);
		m_FwdStrokeNum = 1;
	}
}

void
TXhSyllableSegment::_forwardNumber(unsigned n)
{
}

void
TXhSyllableSegment::_forwardStroke(TSyllable &syllable)
{
    std::list<TrieBranch>::iterator itn, it = m_TrieBranches.begin();
    std::list<TrieBranch>::iterator ite = m_TrieBranches.end();

	for (itn = it++; it != ite; it = itn, itn++) {
		if(!it->newAdded && _forwardBranch(*it, syllable)) {
			m_TrieBranches.erase(it);
		}
	}
}

bool
TXhSyllableSegment::_forwardBranch(TrieBranch &branch,
						TSyllable &syllable)
{
	bool suc = false;
	PathList fwdPaths;

	suc = branch.forward(syllable, m_FwdStrokeNum, m_NumMet, fwdPaths);
	
	if (!suc)
		return false;

	if (!m_NumMet && m_FwdStrokeNum > 1) {
		m_NumMet = true;
	}

	PathList::iterator it = fwdPaths.begin();
	PathList::iterator ite = fwdPaths.end();

	for (it++; it != ite; it++) {
		TrieBranch b(branch);

		b.addPathInfo(*it);
		b.newAdded = true;
		m_TrieBranches.push_back(b);
	}
	Path &p = fwdPaths.front();
	branch.addPathInfo(p);
	return true;
}

void
TXhSyllableSegment::_buildLexiconStates(unsigned i, unsigned j)
{
    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);
    fr.m_type = CLatticeFrame::SYLLABLE;

	CSyllables syls;
	std::vector<unsigned>::iterator sit = m_syllables.begin();
	std::vector<unsigned>::iterator site = m_syllables.end();
	for (; sit != site; sit++) {
		syls.push_back(TSyllable(*sit));
	}

	std::vector<unsigned> seg_path;
	seg_path.push_back(m_start);
	seg_path.push_back(m_start + m_len);

    BranchList::iterator it = m_TrieBranches.begin();
    BranchList::iterator ite = m_TrieBranches.end();
	for (; it != ite; it++) {
		PathNodeList &nodes = (*it).m_Path.getPathNodes();

		PathNodeList::iterator nit = nodes.begin();
		PathNodeList::iterator nite = nodes.end();
        for (; nit != nite; nit++) {
			TXhLexiconState new_lxst = TXhLexiconState(i, (*nit).getTNode(), syls, seg_path);
			fr.m_lexiconStates.push_back(new_lxst);
		}
	}
}

