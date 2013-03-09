#include <iostream>
#include "pathnode.h" 
#include "path.h" 
#include "checkpoint.h" 
#include "ime-core/helper/CInputTrieSource.h" 
#include "common/lexicon/trie.h"
#include "xh/input/xh_data.h"

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
PathNode::isMostPopularStrokeGe(unsigned n)
{
	return m_TNode->m_nMaxStroke >= n;
}

bool
PathNode::getChildrenFromPesudoTNode(TTransUnit *pTrans,
		TThreadNode *psuNode, PathNodeList &children)
{
	//the children of a pesudo node is not pesudo nodes
	unsigned int sz = psuNode->m_nTransfer;
	TTransUnit *ptrans = (TTransUnit *)psuNode->getTrans();
	for (unsigned int i = 0; i < sz; ++i) {
        unsigned u = ptrans[i].m_Unit;
		TThreadNode *pch = (TThreadNode *)CInputTrieSource::m_pTrie->transfer(psuNode, u);
		children.push_back(PathNode(pTrans, pch, m_Level + 1));
	}
	return true;
}

bool
PathNode::getChildren(PathNodeList &children, TSyllable syllable)
{
	unsigned int sz = m_TNode->m_nTransfer;

	if (sz == 0)
		return false;

	bool pattern = CXhData::isPattern(syllable);

    TTransUnit *ptrans = (TTransUnit *)m_TNode->getTrans();
    for (unsigned int i = 0; i < sz; ++i) {
        unsigned u = ptrans[i].m_Unit;
		if (pattern) {
			if (u == syllable) {
				TThreadNode *pch = (TThreadNode *)CInputTrieSource::m_pTrie->transfer(m_TNode, u);
				if (pch->m_bPesudo) {
					getChildrenFromPesudoTNode(&ptrans[i], pch, children);
				} else {
					children.push_back(PathNode(&ptrans[i], pch, m_Level + 1));
				}
				break;
			}
		} else {
			if (CXhData::isCharComp(u)) {
				TThreadNode *pch = (TThreadNode *)CInputTrieSource::m_pTrie->transfer(m_TNode, u);
				children.push_back(PathNode(&ptrans[i], pch, m_Level + 1));
			}
		}
    }
	return true;
}

bool
PathNode::findFirstSubNodeInPartial(TSyllable syllable, PathList &paths)
{
	PathNodeList children;
	if (!getChildren(children, syllable)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {

		if (CXhData::isBoundary(nit->getTransUnit()))
			continue;

		if ((*nit).transFrom(syllable)) {

			(*nit).flag = PathNode::JUSTNOW;
			
			Path path;
			path.add(*nit);
			paths.push_back(path);

		} else {
			
			bool suc = false;
			PathList subPaths;
			suc = (*nit).findFirstSubNodeInPartial(syllable, subPaths);
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
PathNode::tryNextPartial(TSyllable syllable, PathList &paths)
{
	PathNodeList children;
	if (!getChildren(children, syllable)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if (CXhData::isBoundary(nit->getTransUnit())) {
			nit->findFirstSubNodeInPartial(syllable, paths);
		} else {
			nit->tryNextPartial(syllable, paths);
		}
	}
	return !paths.empty();
}

bool
PathNode::findNextSubNode(TSyllable syllable, PathList &paths)
{
	PathNodeList children;
	if (!getChildren(children, syllable)) {
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
					CInputTrieSource::m_pTrie->print(now);
					break;
				}
			}
		}
		CInputTrieSource::m_pTrie->print((*nit).getTNode());
#endif

		if ((*nit).transFrom(syllable)) {

			(*nit).flag = PathNode::JUSTNOW;
			
			Path path;
			path.add(*nit);
			paths.push_back(path);

		} else {
			
			bool suc = false;
			PathList subPaths;
			suc = (*nit).tryNextPartial(syllable, subPaths);
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
	if (!getChildren(children, syllable)) {
		return false;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		int most = (*nit).getTNode()->m_nMaxStroke;
		if (most < num) {
			continue;
		}

		if (CXhData::isBoundary(nit->getTransUnit()))
			continue;

		Path p(path);
		p.add(*nit);
		p.addPseudoHead();
		if (p.checkNumInPath(syllable, num)) {
			paths.push_back(p);
		}

		Path subPath(path);
		subPath.add(*nit);
		(*nit).findAllSubNode(syllable, num, paths, subPath);
	}
	return !paths.empty();
}

