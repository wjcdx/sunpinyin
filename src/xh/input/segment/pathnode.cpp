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
PathNode::getChildren(TSyllable syllable, PathNodeList &children)
{
	unsigned int sz = m_TNode->m_nTransfer;

	if (sz == 0)
		return false;

    TTransUnit* ptrans = (TTransUnit *)m_TNode->getTrans();
    for (unsigned int i = 0; i < sz; ++i) {
        unsigned u = ptrans[i].m_Unit; 
        TThreadNode *pch = (TThreadNode *)CInputTrieSource::m_pTrie->transfer(m_TNode, u);
		children.push_back(PathNode(&ptrans[i], pch));
    }
	return true;
}

bool
PathNode::findNextSubNode(TSyllable syllable, PathList &paths)
{
	PathNodeList children;
	
	if (!getChildren(syllable, children)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
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
PathNode::findAllSubNode(TSyllable syllable, int num, PathList &paths)
{
	PathNodeList children;
	
	if (!getChildren(syllable, children)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if ((*nit).transFrom(syllable)) {
			num -= 1;
		}

		int wnum = (*nit).getTNode()->m_nWordId;
		if (wnum > 0) {
			Path path;
			path.push_front(*nit);
			path.setWordNode(*nit);
			paths.push_back(path);
		}

		bool suc = false;
		PathList subPaths;
		suc = (*nit).findAllSubNode(syllable, num, subPaths);
		if (!suc)
			continue;

		PathList::iterator pit = subPaths.begin();
		PathList::iterator pite = subPaths.end();
		for (; pit != pite; pit++) {
			if ((*pit).getTransNum(syllable) < num)
				continue;
			(*pit).push_front(*nit);
			paths.push_back(*pit);
		}
	}
	return !paths.empty();
}

