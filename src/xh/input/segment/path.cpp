#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "pathnode.h"
#include "path.h"
#include "checkpoint.h"

void
Path::printNodes()
{
	PathNodeList::iterator nit = m_Nodes.begin();
	PathNodeList::iterator nite = m_Nodes.end();
	std::cout << "\tNodes:\t";
	fflush(stdout);
	for (; nit != nite; nit++) {
		if ((*nit).m_Trans) {
			std::cout << (char)(*nit).m_Trans->m_Unit << " ";
			fflush(stdout);
		}
	}
	std::cout << std::endl;
	fflush(stdout);
}

void
Path::printNextMap()
{
	std::cout << "\tNextMap:\t";
	fflush(stdout);
	PathNode *nxt = m_Now;
	while (nxt) {
		if (nxt && nxt->m_Trans) {
			std::cout << (char)nxt->m_Trans->m_Unit << " ";
			fflush(stdout);
			nxt = next(nxt);
		} else {
			break;
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	fflush(stdout);
}

bool
Path::forward(TSyllable syllable, int num, bool pathInfoFull, PathList &paths)
{
	bool suc = false;

	if (pathInfoFull) {

		if (num > 1) {
			suc = checkNumInPath(syllable, num);
			return suc;
		}

		PathNode *cp = next(syllable);
		if (!cp) {
			return false;
		}
		forward();
		return true;
	}

	//if (num == 1) {
	//	suc = m_Now->findNextSubNode(syllable, paths);
	//} else {
		//m_Now will be unconsciously changed by c++
		//PathNode node = *m_Now;
		suc = m_Now->findAllSubNode(syllable, num, paths);
		/*
		PathNodeList::iterator it = m_Nodes.begin();
		PathNodeList::iterator ite = m_Nodes.end();
		for (; it != ite; it++) {
			if ((*it) == node) {
				m_Now = &(*it);
			}
		}
		*/

		if (!suc)
			return false;
		//add into or remove from paths;
		suc = checkNumInPaths(syllable, num, paths);
	//}

	return suc;
}

/**
 * return value:
 * 0: go on iterate;
 * 1: iterate end succeeded;
 * -1: iterate end failed;
 */
int
Path::getRepeaterStatus(int count, CheckPointList &cphooks)
{
	int len = cpset.size();

	if (len < count) {
		return -1;
	}

	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();

	if (len == 1 && count == 1) {
		cphooks.push_back(*it);
		return 1;
	}

	CheckPoint &cp = *it;
	for (cp = *it, it++; it != ite; cp = *it, it++) {
		if (next(cp.m_PNode) == (*it).m_Start) {
			cphooks.push_back(cp);
		} else {
			cphooks.clear();
		}

		/*
		 * a_a_a
		 * size: num of 'a'
		 * count: number of '_';
		 */
		if ((int)cphooks.size() + 1 >= count)
			return 1;
	}
	return 0;
}

void
Path::forwardCheckPoint()
{
	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (; it != ite; it++) {
		(*it).m_PNode = next((*it).m_PNode);
	}
}

int
Path::getSameRepNumber(CheckPoint &cp)
{
	int c = 0;
	PathNode *n1 = cp.m_PNode;

	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (; it != ite; it++) {
		if (*it == cp)
			continue;
		if (n1 == (*it).m_PNode) {
			c++;
		}
	}
	return c;
}

void
Path::iterateRepeaters(int count)
{
	if (cpset.size() <= 1)
		return;
	
	forwardCheckPoint();

	CheckPointList::iterator it, itn = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (it = itn, itn++; it != ite; it = itn, itn++) {
		int c = getSameRepNumber(*it);
		if (c+1 < count) {
			cpset.erase(it);
		}
	}
}

void
Path::findCheckPoints(TSyllable syllable)
{
	cpset.clear();

	PathNode *n = next(m_Now);
	for (; n != NULL; n = next(n)) {
		if (n->transFrom(syllable)) {
			CheckPoint cp(n);
			cpset.push_back(cp);
		}
	}
}

void
Path::labelPath(CheckPointList &cphooks)
{
	assert(!cphooks.empty());
	/*
	if (cphooks.empty())
		return;
	*/

	CheckPoint &cp = cphooks.back();
	cp.m_PNode->setFlag(PathNode::JUSTNOW);
}

bool
Path::checkNumInPath(TSyllable syllable, int num)
{
	int stat = 0;

	findCheckPoints(syllable);
	if ((int)cpset.size() < num)
		return false;

	while (true) {
		CheckPointList cphooks;
		stat = getRepeaterStatus(num, cphooks);
		if (stat == 1) {
			labelPath(cphooks);
			return true;
		} else if (stat == -1) {
			return false;
		}
		iterateRepeaters(num);
	}
	return false;
}

bool
Path::checkNumInPaths(TSyllable syllable, int num, PathList &paths)
{
	PathList::iterator it, itn = paths.begin();
	PathList::iterator ite = paths.end();
	for (it = itn, itn++; it != ite; it = itn, itn++) {
		(*it).resetNowNode();
		if (!(*it).checkNumInPath(syllable, num)) {
			paths.erase(it);
		}
	}
	return !paths.empty();
}

