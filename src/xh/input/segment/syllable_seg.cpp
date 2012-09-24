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
		this.lastCharIsNum = true;
	} else {
		_forwardStroke(syllable);
		this.lastCharIsNum = false;
	}
}

void
TXhSyllableSegment::_forwardNumber(unsigned n)
{
	m_CheckPointNum = n;
}

void
TXhSyllableSegment::_forwardStroke(TSyllable &syllable)
{
    std::vector<TrieBranch>::iterator it = this.m_TrieBranches.begin();
    std::vector<TrieBranch>::iterator ite = this.m_TrieBranches.end();

	for (; it != ite; it++) {
		if(_forwardBranch(*it, syllable)) {
			it->erase();
		}
	}
}

bool
TXhSyllableSegment::_forwardBranch(TrieBranch &branch,
						TSyllable &syllable)
{
    std::vector<CheckPoint>::iterator it = branch.getCheckPoints().begin();
    std::vector<CheckPoint>::iterator ite = branch.getCheckPoints().end();

	for (; it != ite; it++) {
		std::vector<CheckPoint> cpset;

		(*it).forward(syllable, cpset);
		if (cpset.size() == 1) {
			it->historify();
			it->setTNode(cpset.front().m_TNode);
		} else if (cpset.size() > 1 ){
			it->historify();

			std::vector<CheckPoint>::iterator nit = branch.getCheckPoints().begin();
			std::vector<CheckPoint>::iterator nite = branch.getCheckPoints().end();
			for (; nit != nite; nit++) {
				TrieBranch &new_br = branch.copy();
				new_br.getCheckPoints().back().setTNode(nit->m_TNode);
				new_br.collectBranchPathInfo();
				m_TrieBranches.push_bach(new_br);
			}
		} else {
			(*it).erase();
		}
	}

	if (branch.getCheckPoints().size() < branch.getCheckPointNum()) {
		return false;
	} else {
		branch.collectBranchPathInfo();
		return true;
	}
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

