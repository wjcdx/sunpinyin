#include "checkpoint.h" 
bool
PathNode::forward(BranchPath &path, TSyllable syllable, PathVec &paths, int num, bool pathInfoFull)
{
	bool suc = false;

	if (pathInfoFull) {
		PathNode &cp = path.next(syllable);
		if (cp == NULL || cp.isEnd()) {
			return false;
		} 
		path.forward();
		return true;
	}

	if (num == 1) {
		suc = findNextSubNode(m_TNode, syllable, paths);
	} else {
		suc = findAllSubNode(m_TNode, syllable, num, paths);
		if (!suc)
			return false;

		//add into or remove from fwdPathes;
		suc = checkNumInPaths(fwdPaths, syllable, num);
	}

	return suc;
}

bool
PathNode::findNextSubNode(TNode *node, TSyllable syllable, PathVec &paths)
{
	PathNodeVec children = node.getChildren(syllable);
	if (children.empty()) {
		return false;
	}

	PathNodeVec::iterator nit = children.begin();
	PathNodeVec::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if ((*nit).transFrom(syllable)) {
			(*nit).flag = JUSTNOW;

			BranchPath path;
			path.push_front(nit);
			paths.push_back(path);
		} else {
			(*nit).flag = JUMPED;
			
			bool suc = false;
			PathVec subPaths;
			suc = findNextSubNode((*nit).m_TNode, syllable, subPaths);
			if (!suc)
				continue;

			PathVec::iterator pit = subPaths.begin();
			PathVec::iterator pite = subPaths.end();
			for (; pit != pite; pit++) {
				(*pit).push_front(nit);
				paths.push_back(*pit);
			}
		}
	}
	return !paths.empty();
}

bool
PathNode::findAllSubNode(TNode *node, TSyllable syllable, int num, PathVec &paths)
{
	PathNodeVec children = node.getChildren(syllable);
	if (children.empty()) {
		BranchPath path;
		PathNode node;
		node.flag = END;
		path.push_front(node);
		paths.push_back(path);
		return true;
	}

	PathNodeVec::iterator nit = children.begin();
	PathNodeVec::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if ((*nit).transFrom(syllable)) {
			(*nit).flag = FUTURE;
			num -= 1;
		} else {
			(*nit).flag = CHECKPOINT;
		}

		bool suc = false;
		PathVec subPaths;
		suc = findAllSubNode((*nit).m_TNode, syllable, num, subPaths);
		if (!suc)
			continue;

		PathVec::iterator pit = subPaths.begin();
		PathVec::iterator pite = subPaths.end();
		for (; pit != pite; pit++) {
			if ((*pit).getTransNum(syllable) < num)
				continue;
			(*pit).push_front(nit);
			paths.push_back(*pit);
		}
	}
	return !paths.empty();
}

/**
 * return value:
 * 0: go on iterate;
 * 1: iterate end succeeded;
 * -1: iterate end failed;
 */
int
getRepeaterStatus(BranchPath &path, CheckPointVec &cpset, int count, CheckPointVec &cphooks)
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
		if (path.next(cp.m_PNode) == it->m_Start) {
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
forwardCheckPoint(BranchPath &path, CheckPointVec &cpset)
{
	CheckPointVec::iterator it = cpset.begin();
	CheckPointVec::iterator ite = cpset.end();
	for (; it != ite; it++) {
		(*it).m_PNode = path.next((*it).m_PNode);
	}
}

int
getSameRepNumber(BranchPath &path, CheckPoint &cp, CheckPointVec &cpset)
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
iterateRepeaters(BranchPath &path, CheckPointVec &cpset, int count)
{
	if (cpset.size() <= 1)
		return;
	
	forwardCheckPoint(path, cpset);

	CheckPointVec::iterator it = cpset.begin();
	CheckPoint &cp = cpset.back();
	for (; *it != cp;) {
		int c = getSameRepNumber(path, *it, cpset);
		if (c+1 >= count) {
			it++;
		} else {
			(*it).erase();
		}
	}
}

CheckPointVec
findNodesBySyllable(TSyllable syllable)
{
	CheckPointVec cpset;

	PathNodeVec::iterator it = m_Nodes.begin();
	PathNodeVec::iterator ite = m_Nodes.begin();
	for (; it != ite; it++) {
		if ((*it).flag == CHECKPOINT) {
			CheckPoint cp((*it).m_Node);
			cpset.push_back(cp);
		}
	}
	return cpset;
}

void
labelPath(BranchPath &path, CheckPointVec &cpset)
{
	CheckPointVec::iterator it = cpset.begin();
	CheckPointVec::iterator ite = cpset.end();

	CheckPoint &cp = *it;
	PathNode &node = path.getNow();
	for (; node != *it.m_Start; node = path.next(node)) {
		node.flag = JUMPED;
	}

	for (; it != ite; it++) {
		cp = *it;
		node = cp.Start;
		for (; node != cp.m_PNode; node = path.next(node)) {
			node.flag = HISTORY;
		}
	}
	cp.m_PNode.flag = JUSTNOW;
}

bool
checkNumInPath(BranchPath &path, TSyllable syllable, int num)
{
	int stat = 0;
	CheckPointVec cpset = path.findNodesBySyllable(syllable);
	if (cpset.size() < num)
		return false;

	while (true) {
		CheckPointVec cphooks;
		stat = getRepeaterStatus(path, cpset, num, cphooks);
		if (stat == 1) {
			lablePath(path, cpset);
			return true;
		} else if (stat == -1) {
			return false;
		}
		iterateRepeaters(path, cpset, num);
	}
	return false;
}

bool
PathNode::checkNumInPaths(PathVec paths, TSyllable syllable, int num)
{
	PathVec::iterator it = paths.begin();
	PathVec::iterator ite = paths.end();
	for (; it != ite; it++) {
		BranchPath &path = *it;
		if (!checkNumInPath(path, syllable, num)) {
			path.erase();
		}
	}
	return !paths.empty();
}

