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
	for (; it != ite; it++) {
		TXhLexiconState &lxst = *(TXhLexiconState *)(&(*it));

        if (lxst.m_pNode) {
            // try to match a word from lattice i to lattice j
            // and if match, we'll count it as a new lexicon on lattice j
			_forwardFromLastSegment(jfr, lxst);
        }
		_forwardFromRoot(i, jfr);
	}
}

void
TXhSyllableSegment::prepare(const TThreadNode *pTNode)
{
	m_TrieBranches.clear();
	PathNode node(NULL, const_cast<TThreadNode *>(pTNode), PathNode::JUSTNOW);
	TrieBranch branch;
	branch.setNewAdded(false);
	branch.m_Path.add(node);
	m_TrieBranches.push_back(branch);
	m_FwdStrokeNum = 1;
	m_bNumMet = false;
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

			TXhLexiconState new_lxst = TXhLexiconState(lxst.m_start,
												   bit->getPath().getNow()->getTNode(),
												   lxst.m_syls,
												   lxst.m_seg_path);
			new_lxst.m_syls.push_back(m_syllables[0]);
			new_lxst.m_seg_path.push_back(m_start + m_len);
			jfr.m_lexiconStates.push_back(new_lxst);
		}
	}
}

void TXhSyllableSegment::_forwardFromRoot(unsigned i, CLatticeFrame &jfr)
{
	_forwardFromTNode((TrieThreadModel::TThreadNode*)
			CInputTrieSource::m_pTrie->getRootNode());

	if (!m_TrieBranches.empty()) {
		BranchList::iterator bit = m_TrieBranches.begin();
		BranchList::iterator bite = m_TrieBranches.end();

		for (; bit != bite; bit++) {
			CSyllables syls;
			syls.push_back(m_syllables[0]);
			std::vector<unsigned> seg_path;
			seg_path.push_back(m_start);
			seg_path.push_back(m_start + m_len);
			TXhLexiconState new_lxst = TXhLexiconState(i, 
									bit->getPath().getNow()->getTNode(),
									syls, seg_path);
			jfr.m_lexiconStates.push_back(new_lxst);
		}
	}
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

/*
void
TXhSyllableSegment::_buildForSingleSyllable(CLatticeFrame &ifr,
		CLatticeFrame &jfr, TSyllable syllable, CLexiStateMap &statesMap)
{
    const TThreadNode * pn = NULL;

	CLexiconStates::iterator it = ifr.m_lexiconStates.begin();
	CLexiconStates::iterator ite = ifr.m_lexiconStates.end();

	if (it != ite) {
		TXhLexiconState &lxst = *(TXhLexiconState *)(&(*it));
		LexiStateKey key(lxst.m_start, lxst.m_syls, lxst.m_seg_path);

		TThreadNodeVec::iterator nit = lxst.m_nodes.begin();
		TThreadNodeVec::iterator nite = lxst.m_nodes.end();
		for (; nit != nite; nit++) {
			// try to match a word from lattice i to lattice j
			// and if match, we'll count it as a new lexicon on lattice j
			pn = CInputTrieSource::m_pTrie->transfer(*nit, syllable);
			if (pn) {
				statesMap[key].push_back(pn);
			}
		}
	}
   
	// last, create a lexicon for single character with only one syllable
    pn = CInputTrieSource::m_pTrie->transfer(syllable);
    if (pn) {
		LexiStateKey key(m_start);
		statesMap[key].push_back(pn);
    }
}

void
TXhSyllableSegment::_buildLexiconStates(unsigned i, unsigned j)
{
    CLatticeFrame &ifr = CLatticeManager::getLatticeFrame(i);
    CLatticeFrame &jfr = CLatticeManager::getLatticeFrame(j);
    jfr.m_type = CLatticeFrame::SYLLABLE;

	CLexiStateMap statesMap;

    BranchList::iterator it = m_TrieBranches.begin();
    BranchList::iterator ite = m_TrieBranches.end();
	
	for (; it != ite; it++) {
		TThreadNode *pn = it->getPath().getNow()->getTNode();
		unsigned int sz = pn->m_nWordId;
		const TWordIdInfo *pwids = pn->getWordIdPtr();
		for (unsigned int idx = 0; idx < sz; idx++) {
			_buildForSingleSyllable(ifr, jfr, pwids[idx].m_id, statesMap);
		}
	}
	
	CLexiStateMap::iterator mit = statesMap.begin();
	CLexiStateMap::iterator mite = statesMap.end();
	for (; mit != mite; mit++) {
		LexiStateKey &key = const_cast<LexiStateKey &>(mit->first);
		TThreadNodeVec &nodes = mit->second;
		TXhLexiconState new_lxst = TXhLexiconState(
										key.m_start,
										(const TThreadNode *)NULL,
										key.m_syls,
										key.m_seg_path);
		
		new_lxst.m_syls.push_back(0);
		new_lxst.m_seg_path.push_back(m_start + m_len);

		TThreadNodeVec::iterator nit = nodes.begin();
		TThreadNodeVec::iterator nite = nodes.end();
		//int idx = 0;
		for (; nit != nite; nit++) {
			//if (++idx > 40) {
			//	break;
			//}
			new_lxst.m_nodes.push_back(*nit);
			new_lxst.m_words.push_back(*(*nit)->getWordIdPtr());
		}
		jfr.m_lexiconStates.push_back(new_lxst);
		printf("segment %u nodes start: %u\tsize: %d\n", i,
				key.m_start, (int)new_lxst.m_nodes.size());
	}
	printf("segment %u jfr.lxst.size: %d\n", i, (int)jfr.m_lexiconStates.size());
}
*/

