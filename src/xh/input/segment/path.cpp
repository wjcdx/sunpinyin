#include "pathnode.h"
#include "path.h"
#include "checkpoint.h"
	
bool
Path::forward(TSyllable syllable, int num, bool pathInfoFull, PathVec &paths)
{
	bool suc = false;

	if (pathInfoFull) {
		PathNode &cp = this.next(syllable);
		if (cp == NULL || cp.isEnd()) {
			return false;
		} 
		this.forward();
		return true;
	}

	if (num == 1) {
		suc = m_Now.findNextSubNode(syllable, paths);
	} else {
		suc = m_Now.findAllSubNode(syllable, num, paths);
		if (!suc)
			return false;

		//add into or remove from fwdPathes;
		suc = checkNumInPaths(fwdPaths, syllable, num);
	}

	return suc;
}

/**
 * return value:
 * 0: go on iterate;
 * 1: iterate end succeeded;
 * -1: iterate end failed;
 */
int
Path::getRepeaterStatus(int count, CheckPointVec &cphooks)
{
	int c = 0;
	int len = cpset.size();

	if (len < count) {
		return -1;
	}

	if (len == 1 && count == 1)
		return 1;

	CheckPointVec::iterator it = cpset.begin();
	CheckPointVec::iterator ite = cpset.end();
	CheckPoint &cp = *it;
	for (cp = *it, it++; it != ite; cp = *it, it++) {
		if (this.next(cp.m_PNode) == it->m_Start) {
			c++;
			cphooks.push_back(cp);
		} else {
			c = 0;
			cphooks.clear();
		}

		if (c+1 >= count)
			return 1;
	}
	return 0;
}

void
Path::forwardCheckPoint()
{
	CheckPointVec::iterator it = cpset.begin();
	CheckPointVec::iterator ite = cpset.end();
	for (; it != ite; it++) {
		(*it).m_PNode = this.next((*it).m_PNode);
	}
}

int
Path::getSameRepNumber(CheckPoint &cp)
{
	int c = 0;
	PathNode *n1 = cp.m_PNode;

	CheckPointVec::iterator it = cpset.begin();
	CheckPointVec::iterator ite = cpset.end();
	for (; it != ite; it++) {
		if (*it = cp)
			continue;
		if (n1 == it->m_PNode) {
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

	CheckPointVec::iterator it = cpset.begin();
	CheckPoint &cp = cpset.back();
	for (; *it != cp;) {
		int c = getSameRepNumber(*it);
		if (c+1 >= count) {
			it++;
		} else {
			(*it).erase();
		}
	}
}

void
Path::findCheckPoints(TSyllable syllable)
{
	PathNodeVec::iterator it = m_Nodes.begin();
	PathNodeVec::iterator ite = m_Nodes.begin();
	for (; it != ite; it++) {
		if ((*it).flag == CHECKPOINT) {
			CheckPoint cp(*it);
			cpset.push_back(cp);
		}
	}
}

void
Path::labelPath(CheckPointVec &cphooks)
{
	CheckPointVec::iterator it = cphooks.begin();
	CheckPointVec::iterator ite = cphooks.end();

	CheckPoint &cp = *it;
	PathNode &node = getNow();
	for (; node != *it.m_Start; node = this.next(node)) {
		node.flag = JUMPED;
	}

	for (; it != ite; it++) {
		cp = *it;
		node = cp.Start;
		for (; node != cp.m_PNode; node = this.next(node)) {
			node.flag = HISTORY;
		}
	}
	cp.m_PNode.flag = JUSTNOW;
}

bool
Path::checkNumInPath(TSyllable syllable, int num)
{
	int stat = 0;

	this.findCheckPoints(syllable);
	if (cpset.size() < num)
		return false;

	while (true) {
		CheckPointVec cphooks;
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
Path::checkNumInPaths(PathVec paths, TSyllable syllable, int num)
{
	PathVec::iterator it = paths.begin();
	PathVec::iterator ite = paths.end();
	for (; it != ite; it++) {
		Path &path = *it;
		if (!path.checkNumInPath(syllable, num)) {
			paths.erase(path);
		}
	}
	return !paths.empty();
}

