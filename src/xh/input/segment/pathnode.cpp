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
PathNode::findAllSubNode(TSyllable syllable, int num, PathList &paths, Path &path)
{
	PathNodeList children;
	if (!getChildren(syllable, children)) {
		int most = getTNode()->m_nMaxStroke;
		if (most < num) {
			return false;
		}

		path.addPseudoHead();
		if (path.checkNumInPath(syllable, num)) {
			paths.push_back(path);
			return true;
		}
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		int most = (*nit).getTNode()->m_nMaxStroke;
		if (most < num) {
			return false;
		}

		bool has = (*nit).getTNode()->hasItsOwnWord();
		if (has) {
			Path p = path;
			p.addPseudoHead();
			if (p.checkNumInPath(syllable, num)) {
				paths.push_back(p);
				return true;
			}
		}

		Path subPath = path;
		subPath.add(*nit);
		(*nit).findAllSubNode(syllable, num, paths, subPath);
	}
	return !paths.empty();
}

