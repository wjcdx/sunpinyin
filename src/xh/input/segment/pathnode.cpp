#include <iostream>
#include "pathnode.h" 
#include "path.h" 
#include "checkpoint.h" 
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
		TThreadNode *pch = (TThreadNode *)m_pInputTrieSrc->getTrie()->transfer(psuNode, u);
		PathNode node(pTrans, pch, m_Level + 1);
		node.setInputTrieSource(m_pInputTrieSrc);
		children.push_back(node);
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
				TThreadNode *pch = (TThreadNode *)m_pInputTrieSrc->getTrie()->transfer(m_TNode, u);
				if (pch->m_bPesudo) {
					getChildrenFromPesudoTNode(&ptrans[i], pch, children);
				} else {
					PathNode node(&ptrans[i], pch, m_Level + 1);
					node.setInputTrieSource(m_pInputTrieSrc);
					children.push_back(node);
				}
				break;
			}
		} else {
			if (CXhData::isCharComp(u)) {
				TThreadNode *pch = (TThreadNode *)m_pInputTrieSrc->getTrie()->transfer(m_TNode, u);

				PathNode node(&ptrans[i], pch, m_Level + 1);
				node.setInputTrieSource(m_pInputTrieSrc);
				children.push_back(node);
			}
		}
    }
	return !children.empty();
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

		// there should be at least one stroke
		// inputed in a partial.
		if (CXhData::isBoundary(nit->getTransUnit()))
			continue;

		if (this->isInLastPartial()) {
			nit->setInLastPartial(true);
		}

		if ((*nit).transFrom(syllable)) {

			(*nit).flag = PathNode::JUSTNOW;
			
			Path path;
			path.add(*nit);
			paths.push_back(path);

		} else {
			if (CXhData::isFirstStroke(syllable) && this->isFirstInPartial())
				continue;
			
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

			nit->setFirstInPartial(true);

			if (CXhData::isLastBoundary(nit->getTransUnit())) {
				nit->setInLastPartial(true);
			}

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
		
		bool suc = false;
		PathList subPaths;

		// pattern syllables go here
		if (nit->transFrom(syllable)) {

			if (CXhData::isPattern(nit->getTransUnit())) {

				(*nit).flag = PathNode::JUSTNOW;
				
				Path path;
				path.add(*nit);
				paths.push_back(path);

			} else if (this->isInLastPartial()) {

				(*nit).flag = PathNode::JUSTNOW;
				nit->setInLastPartial(true);
				
				Path path;
				path.add(*nit);
				paths.push_back(path);

			} else {
				// only the last partial has multi-keyes.
				suc = nit->tryNextPartial(syllable, subPaths);
			}

		} else {
			// syllable must be the next
			// stroke in last partial.
			if (this->isInLastPartial()) {
				continue;
			}

			if (CXhData::isBoundary(nit->getTransUnit())) {

				nit->setFirstInPartial(true);
				if (CXhData::isLastBoundary(nit->getTransUnit())) {
					nit->setInLastPartial(true);
				}

				suc = nit->findFirstSubNodeInPartial(syllable, subPaths);

			} else {
				suc = nit->tryNextPartial(syllable, subPaths);
			}

		}

		if (!suc)
			continue;

		PathList::iterator pit = subPaths.begin();
		PathList::iterator pite = subPaths.end();
		for (; pit != pite; pit++) {
			(*pit).push_front(*nit);
			paths.push_back(*pit);
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

