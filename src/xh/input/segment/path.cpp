#include "pathnode.h"
#include "path.h"
#include "checkpoint.h"
	
bool
Path::forward(TSyllable syllable, int num, bool pathInfoFull, PathList &paths)
{
	bool suc = false;

	if (pathInfoFull) {
		PathNode *cp = next(syllable);
		if (cp == NULL || cp->isEnd()) {
			return false;
		} 
		this->forward();
		return true;
	}

	//if (num == 1) {
	//	suc = m_Now->findNextSubNode(syllable, paths);
	//} else {
		suc = m_Now->findAllSubNode(syllable, num, paths);
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
	int c = 0;
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
	PathNodeList::iterator it = m_Nodes.begin();
	PathNodeList::iterator ite = m_Nodes.end();
	for (; it != ite; it++) {
		if ((*it).flag == PathNode::CHECKPOINT) {
			CheckPoint cp(&(*it));
			cpset.push_back(cp);
		}
	}
}

void
Path::labelPath(CheckPointList &cphooks)
{
	if (cphooks.empty())
		return;

	CheckPointList::iterator it = cphooks.begin();
	CheckPointList::iterator ite = cphooks.end();

	CheckPoint &cp = *it;
	PathNode *node = getNow();
	for (; node != (*it).m_Start; node = next(node)) {
		node->flag = PathNode::JUMPED;
	}

	for (; it != ite; it++) {
		cp = *it;
		node = cp.m_Start;
		for (; node != cp.m_PNode; node = next(node)) {
			node->flag = PathNode::HISTORY;
		}
	}
	cp.m_PNode->flag = PathNode::JUSTNOW;
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
		(*it).initNodeNow();
		if (!(*it).checkNumInPath(syllable, num)) {
			paths.erase(it);
		}
	}
	return !paths.empty();
}

