#include "common/input/segment/syllable_seg.h"
#include "xh/lattice/TXhLexiconState.h"
#include "ime-core/lattice/lattice.h"
#include "ime-core/lattice/lattice_manager.h"
#include "TXhSyllableSegment.h"

using namespace TrieThreadModel;

void
TXhSyllableSegment::forward(unsigned i, unsigned j)
{
    CLatticeFrame &ifr = CLatticeManager::getLatticeFrame(i);
    CLatticeFrame &jfr = CLatticeManager::getLatticeFrame(j);
    jfr.m_type = CLatticeFrame::SYLLABLE;

	CLexiconStates::iterator it = ifr.m_lexiconStates.begin();
	CLexiconStates::iterator ite = ifr.m_lexiconStates.end();

	unsigned max = 0;
	if (it != ite) {
		max = (*it)->m_syls.size();
	}

	for (; it != ite; it++) {

		TXhLexiconState &lxst = *(TXhLexiconState *)((*it));

		// just get the longest words, others will be
		// rebuilded after the front ones are selected.
		// The 'rebuild' here does research the existing
		// lexicon states, but not re-forward segments.
		// re-forwarding costs too much.
		if (lxst.m_syls.size() < max)
			break;

        if (lxst.m_pNode) {
            // try to match a word from lattice i to lattice j
            // and if match, we'll count it as a new lexicon on lattice j
			_forwardFromLastSegment(jfr, lxst);
        }
	}
	_forwardFromRoot(i, jfr);
}

void
TXhSyllableSegment::prepare(const TThreadNode *pTNode)
{
	m_TrieBranches.clear();
	
	TrieBranch branch;
	branch.setNewAdded(false);
	
	PathNode node(NULL, const_cast<TThreadNode *>(pTNode), 0, PathNode::JUSTNOW);
	node.setInputTrieSource(m_pInputTrieSrc);
	branch.m_Path.add(node);

	m_TrieBranches.push_back(branch);

	m_FwdStrokeNum = 1;
}

void TXhSyllableSegment::_forwardFromTNode(const TThreadNode *pTNode)
{
	prepare(pTNode);
    std::vector<unsigned>::const_iterator it = m_syllables.begin();
    std::vector<unsigned>::const_iterator ite = m_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(*it);
}

void TXhSyllableSegment::_forwardFromLastSegment(CLatticeFrame &jfr,
		TXhLexiconState &lxst)
{
	_forwardFromTNode(lxst.m_pNode);

	if (!m_TrieBranches.empty()) {
		BranchList::iterator bit = m_TrieBranches.begin();
		BranchList::iterator bite = m_TrieBranches.end();

		for (; bit != bite; bit++) {

			PathNode *pnow = bit->getPath().getNow();

			TXhLexiconState *new_lxst = new TXhLexiconState(lxst.m_start,
												   pnow->getTNode(),
												   lxst.m_syls,
												   lxst.m_seg_path);
			new_lxst->setClass(pnow->getLevel());
			new_lxst->m_syls.push_back(m_syllables[0]);
			new_lxst->m_seg_path.push_back(m_start + m_len);
			jfr.m_lexiconStates.push_back(new_lxst);
#if 1
			{
				TThreadNode *now = bit->getPath().getNow()->getTNode();
				m_pInputTrieSrc->getTrie()->print(now);
			}
#endif
		}
	}
}

void TXhSyllableSegment::_forwardFromRoot(unsigned i, CLatticeFrame &jfr)
{
	_forwardFromTNode((TrieThreadModel::TThreadNode*)
			m_pInputTrieSrc->getTrie()->getRootNode());

	if (!m_TrieBranches.empty()) {
		BranchList::iterator bit = m_TrieBranches.begin();
		BranchList::iterator bite = m_TrieBranches.end();

		for (; bit != bite; bit++) {

			CSyllables syls;
			syls.push_back(m_syllables[0]);
			std::vector<unsigned> seg_path;
			seg_path.push_back(m_start);
			seg_path.push_back(m_start + m_len);

			PathNode *pnow = bit->getPath().getNow();

			TXhLexiconState *new_lxst = new TXhLexiconState(i, 
									pnow->getTNode(),
									syls, seg_path);
			new_lxst->setClass(pnow->getLevel());
			jfr.m_lexiconStates.push_back(new_lxst);
#if 1
			{
				TThreadNode *now = bit->getPath().getNow()->getTNode();
				m_pInputTrieSrc->getTrie()->print(now);
			}
#endif
		}
	}
}

void
TXhSyllableSegment::_forwardSingleSyllable(TSyllable syllable)
{
	if (isdigit(syllable)) {
		m_FwdStrokeNum = syllable - '0';
		_forwardNumber(m_FwdStrokeNum);
	} else {
		_forwardStroke(syllable);
		m_FwdStrokeNum = 1;
	}
}

void
TXhSyllableSegment::_forwardNumber(unsigned n)
{
    std::list<TrieBranch>::iterator it = m_TrieBranches.begin();

	fprintf(stdout, "N: %lu\n", m_TrieBranches.size());
	for (; it != m_TrieBranches.end();) {
		PathNode *pnode = it->getPath().getNow();
		if (pnode->isMostPopularStrokeGe(n)) {
			it++;
		} else {
			it = m_TrieBranches.erase(it);
		}
	}
	fprintf(stdout, "N: %lu\n\n", m_TrieBranches.size());
}

void
TXhSyllableSegment::_forwardStroke(TSyllable &syllable)
{
    std::list<TrieBranch>::iterator it = m_TrieBranches.begin();

	fprintf(stdout, "N: %lu\n", m_TrieBranches.size());
	for (; it != m_TrieBranches.end();) {
		if (it->isNewAdded()) {
			it->setNewAdded(false);
			it++;
		} else {
			if (!_forwardBranch(*it, syllable)) {
				it = m_TrieBranches.erase(it);
			} else {
				it++;
			}
		}
	}
	fprintf(stdout, "N: %lu\n\n", m_TrieBranches.size());
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
			//if (id == 765) {
				m_pInputTrieSrc->getTrie()->print(now);
				//break;
			//}
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

