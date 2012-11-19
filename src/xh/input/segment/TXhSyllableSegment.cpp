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
	TrieBranch branch;
	branch.setNewAdded(false);
	branch.m_Path.add(node);
	m_TrieBranches.push_back(branch);
	m_FwdStrokeNum = 1;
	m_bNumMet = false;
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

	fprintf(stdout, "N: %lu\n", m_TrieBranches.size());
	for (it = itn, itn++; it != m_TrieBranches.end();
			it = itn, itn++) {
		if (it->isNewAdded()) {
			it->setNewAdded(false);
		} else {
			if (!_forwardBranch(*it, syllable)) {
				m_TrieBranches.erase(it);
			}
		}
	}
	fprintf(stdout, "N: %lu\n", m_TrieBranches.size());
}

bool
TXhSyllableSegment::_forwardBranch(TrieBranch &branch,
						TSyllable &syllable)
{
	bool forwarded = false;

#if 0
	{
		TThreadNode *now = branch.getPath().getNow()->getTNode();
		unsigned int sz = now->m_nWordId;
		const TWordIdInfo *pwids = now->getWordIdPtr();
		for (unsigned int i = 0; i < sz; ++i) {
			unsigned int id = pwids[i].m_id;
			if (id == 765) {
				CInputTrieSource::m_pTrie->print(now);
				break;
			}
		}
	}
#endif

	PathList fwdPaths;
	forwarded = branch.forward(syllable, m_FwdStrokeNum, fwdPaths);
	
	if (!forwarded)
		return false;

	PathList::iterator it = fwdPaths.begin();
	PathList::iterator ite = fwdPaths.end();

	for (it++; it != ite; it++) {
		TrieBranch b(branch);

		b.setNewAdded(true);
		b.addPathInfo(*it);
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
		//CInputTrieSource::m_pTrie->print((*it).getPath().getNow()->getTNode());
		TXhLexiconState new_lxst = TXhLexiconState(i,
			it->getPath().getNow()->getTNode(),
			syls, seg_path);
		fr.m_lexiconStates.push_back(new_lxst);
	}
}

