#include <iostream>
#include "pathnode.h" 
#include "path.h" 
#include "checkpoint.h" 
#include "ime-core/helper/CInputTrieSource.h" 
#include "common/lexicon/trie.h" 

using namespace TrieThreadModel;

void
PathNode::print()
{
	if (m_Trans) {
		std::cout << (char)m_Trans->m_Unit << " ";
		std::cout << std::endl;
		fflush(stdout);
	}
}

bool
PathNode::getChildren(PathNodeList &children)
{
	unsigned int sz = m_TNode->m_nTransfer;

	if (sz == 0)
		return false;

    TTransUnit* ptrans = (TTransUnit *)m_TNode->getTrans();
    for (unsigned int i = 0; i < sz; ++i) {
        unsigned u = ptrans[i].m_Unit; 
        TThreadNode *pch = (TThreadNode *)CInputTrieSource::m_pTrieOc->transfer(m_TNode, u);
		children.push_back(PathNode(&ptrans[i], pch));
    }
	return true;
}

bool
PathNode::findNextSubNode(TSyllable syllable, PathList &paths)
{
	PathNodeList children;
	if (!getChildren(children)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
#if 0
		{
			TThreadNode *now = (*nit).getTNode();
			unsigned int sz = now->m_nWordId;
			const TWordIdInfo *pwids = now->getWordIdPtr();
			for (unsigned int i = 0; i < sz; ++i) {
				unsigned int id = pwids[i].m_id;
				if (id == 765) {
					CInputTrieSource::m_pTrieOc->print(now);
					break;
				}
			}
		}
		CInputTrieSource::m_pTrieOc->print((*nit).getTNode());
#endif

		if ((*nit).transFrom(syllable)) {
			(*nit).flag = PathNode::JUSTNOW;
			
			Path path;
			path.add(*nit);
			paths.push_back(path);
		} else {
			
			bool suc = false;
			PathList subPaths;
			suc = (*nit).findNextSubNode(syllable, subPaths);
			if (!suc)
				continue;

			PathList::iterator pit = subPaths.begin();
			PathList::iterator pite = subPaths.end();
			for (; pit != pite; pit++) {
				(*pit).push_front(*nit);
				paths.push_back(*pit);
			}
		}
	}
	return !paths.empty();
}

bool
PathNode::findAllSubNode(TSyllable syllable, int num, PathList &paths, Path &path)
{
	PathNodeList children;
	if (!getChildren(children)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		int most = (*nit).getTNode()->m_nMaxStroke;
		if (most < num) {
			continue;
		}

		bool has = (*nit).getTNode()->hasItsOwnWord();
		if (has) {
			Path p(path);
			p.add(*nit);
			p.addPseudoHead();
			if (p.checkNumInPath(syllable, num)) {
				p.setFullForwarded(true);
				paths.push_back(p);
			}
		}

		Path subPath(path);
		subPath.add(*nit);
		(*nit).findAllSubNode(syllable, num, paths, subPath);
	}
	return !paths.empty();
}

