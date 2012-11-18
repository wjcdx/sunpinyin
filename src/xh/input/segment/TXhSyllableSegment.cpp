#include "common/input/segment/syllable_seg.h"
#include "xh/lattice/TXhLexiconState.h"
#include "ime-core/lattice/lattice.h"
#include "ime-core/lattice/lattice_manager.h"
#include "TXhSyllableSegment.h"

unsigned int TXhSyllableSegment::m_FwdedBranchNum = 0;
const unsigned int TXhSyllableSegment::MAX_FWD_BRANCH_NUM = 20;

void
TXhSyllableSegment::prepare()
{
	m_TrieBranches.clear();
	PathNode node(NULL,
		(TrieThreadModel::TThreadNode*)CInputTrieSource::m_pTrie->getRootNode(),
		PathNode::JUSTNOW);
	TrieBranch branch;
	branch.m_Path.add(node);
	m_TrieBranches.push_back(branch);
	m_FwdStrokeNum = 1;
	m_FwdedBranchNum = 0;
}

void
TXhSyllableSegment::forward(unsigned i, unsigned j)
{
	prepare();
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
		m_FwdStrokeNum = syllable - '0';
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
    std::list<TrieBranch>::iterator it, itn = m_TrieBranches.begin();
    std::list<TrieBranch>::iterator ite = m_TrieBranches.end();

	for (it = itn, itn++; it != ite; it = itn, itn++) {
		if (!_forwardBranch(*it, syllable)) {
			if ((*it).isFullForwarded()) {
				m_FwdedBranchNum--;
			}
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
	suc = branch.forward(syllable, m_FwdStrokeNum, m_bNumMet, fwdPaths);
	
	if (!suc)
		return false;

	if (!m_bNumMet) {
		if (m_FwdedBranchNum > 1)
			m_bNumMet = true;
	} else {
		return true;
	}

	PathList::iterator it = fwdPaths.begin();
	PathList::iterator ite = fwdPaths.end();

	for (it++; it != ite; it++) {
		TrieBranch b(branch);

		b.addPathInfo(*it);
		m_TrieBranches.push_back(b);
		if (b.isFullForwarded()) {
			m_FwdedBranchNum++;
		}
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
		TXhLexiconState new_lxst = TXhLexiconState(i,
			(*it).getPath().getWordNode().getTNode(),
			syls, seg_path);
		fr.m_lexiconStates.push_back(new_lxst);
	}
}

