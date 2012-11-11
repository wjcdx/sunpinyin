#include "pathnode.h" 
#include "path.h" 
#include "checkpoint.h" 
#include "ime-core/helper/CInputTrieSource.h" 
#include "common/lexicon/trie.h" 

using namespace TrieThreadModel;

PathNodeList
PathNode::getChildren(TSyllable syllable)
{
	PathNodeList nodes;

	unsigned int sz = m_TNode->m_nTransfer;

	if (sz == 0)
		return nodes;

    TTransUnit* ptrans = (TTransUnit *)m_TNode->getTrans();
    for (unsigned int i = 0; i < sz; ++i) {
        unsigned u = ptrans[i].m_Unit; 
        TThreadNode *pch = (TThreadNode *)CInputTrieSource::m_pTrie->transfer(m_TNode, u);
		nodes.push_back(PathNode(&ptrans[i], pch));
    }
	return nodes;
}

bool
PathNode::findNextSubNode(TSyllable syllable, PathList &paths)
{
	PathNodeList children = getChildren(syllable);
	if (children.empty()) {
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
			(*nit).flag = PathNode::JUMPED;
			
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
	PathNodeList children = getChildren(syllable);
	if (children.empty()) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if ((*nit).transFrom(syllable)) {
			(*nit).flag = PathNode::CHECKPOINT;
			num -= 1;
		} else {
			(*nit).flag = PathNode::FUTURE;
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

