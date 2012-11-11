#include "common/input/segment/syllable_seg.h"
#include "xh/lattice/TXhLexiconState.h"
#include "ime-core/lattice/lattice.h"
#include "ime-core/lattice/lattice_manager.h"
#include "TXhSyllableSegment.h"


void
TXhSyllableSegment::prepare()
{
	m_TrieBranches.clear();
	PathNode node(NULL,
			(TrieThreadModel::TThreadNode*)CInputTrieSource::m_pTrie->getRootNode(),
			PathNode::JUSTNOW);
	Path path(node);
	TrieBranch branch;
	branch.m_Path = path;
	//branch.newAdded = false;
	m_TrieBranches.push_back(branch);
	m_NumMet = false;
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
    std::list<TrieBranch>::iterator it, itn = m_TrieBranches.begin();
    std::list<TrieBranch>::iterator ite = m_TrieBranches.end();

	for (it = itn, itn++; it != ite; it = itn, itn++) {
		//newAdded maybe not needed
		//because newAdded is behind
		//ite, which is not changed
		if (!it->newAdded) {
			if (!_forwardBranch(*it, syllable)) {
				//m_TrieBranches.erase(it);
				it->newAdded = true;
			}
		//} else {
		//	it->newAdded = false;
		}
	}
}

bool
TXhSyllableSegment::_forwardBranch(TrieBranch &branch,
						TSyllable &syllable)
{
	bool suc = false;
	PathList fwdPaths;
	
	branch.getPath().printNodes();
	branch.getPath().printNextMap();

	suc = branch.forward(syllable, m_FwdStrokeNum, m_NumMet, fwdPaths);
	
	if (!suc)
		return false;

	//if (!m_NumMet && m_FwdStrokeNum > 1) {
	if (!m_NumMet) {
		m_NumMet = true;
	}

	PathList::iterator it = fwdPaths.begin();
	PathList::iterator ite = fwdPaths.end();

	for (it++; it != ite; it++) {
		TrieBranch b;

		b.addPathInfo(branch.getPath());
		b.addPathInfo(*it);
		b.newAdded = false;
		//
		b.getPath().printNodes();
		b.getPath().printNextMap();

		m_TrieBranches.push_back(b);
	}
	Path &p = fwdPaths.front();
	branch.addPathInfo(p);
	branch.getPath().printNodes();
	branch.getPath().printNextMap();
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
	/*
		PathNodeList &nodes = (*it).m_Path.getPathNodes();

		PathNodeList::iterator nit = nodes.begin();
		PathNodeList::iterator nite = nodes.end();
        for (; nit != nite; nit++) {
			if ((*nit).getTNode()->m_nWordId <= 0) {
				continue;
			}
			TXhLexiconState new_lxst = TXhLexiconState(i, (*nit).getTNode(), syls, seg_path);
			fr.m_lexiconStates.push_back(new_lxst);
		}
	*/
	}
}

