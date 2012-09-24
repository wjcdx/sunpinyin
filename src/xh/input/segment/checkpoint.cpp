#include "checkpoint.h" 
bool
CheckPoint::forward(TSyllable syllable, PathVec &paths, int num)
{
	PathVec fwdPaths;

	findSubNode(m_TNode, syllable, fwdPaths, num);
	//add into or remove from fwdPathes;
	checkNumInPath(fwdPaths, syllable, num);

	PathVec::iterator it = fwdPaths.begin();
	PathVec::iterator ite = fwdPaths.end();

	for (; it != ite; it++) {
		paths.push_back(fwdPaths);
	}
	return true;
}

bool
CheckPoint::findSubNode(TNode *node, TSyllable syllable, PathVec &paths, int num)
{
	std::vector<TNode *> children = node.getChildren(syllable);

	std::vector<TNode *>::iterator nit = children.begin();
	std::vector<TNode *>::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if (nit->syllable == syllable) {
			num -= 1;
			if (num > 0) {
				PathVec subPaths;
				findSubNode(*nit, syllable, subPaths, num);

				std::vector<BranchPath>::iterator pit = subPaths.begin();
				std::vector<BranchPath>::iterator pite = subPaths.end();
				for (; pit != pite; pit++) {
					(*pit).push_front(nit);
					paths.push_back(*pit);
				}
			} else {
				BranchPath path;
				path.push_front(*nit);
				paths.push_back(path);
				return true;
			}
		} else {
			PathVec subPaths;
			findSubNode(*nit, syllable, subPaths, num);

			std::vector<BranchPath>::iterator pit = subPaths.begin();
			std::vector<BranchPath>::iterator pite = subPaths.end();
			for (; pit != pite; pit++) {
				(*pit).push_front(nit);
				paths.push_back(*pit);
			}
		}
	}
	return false;
}

bool
hasCountPiecesInFirstHalf(int count, int num)
{
	return ((count + 1) >= num);
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
		if (path.next(cp.m_TNode) == it->m_Start) {
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
		(*it).m_TNode = path.next((*it).m_TNode);
	}
}

int
getSameRepNumber(BranchPath &path, CheckPoint &cp, CheckPointVec &cpset)
{
	int c = 0;
	TNode *n1 = cp.m_TNode;

	CheckPointVec::iterator it = cpset.begin();
	CheckPointVec::iterator ite = cpset.end();
	for (; it != ite; it++) {
		if (*it = cp)
			continue;
		if (n1 == it->m_TNode) {
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

bool
findSubPath(BranchPath &path, TNode *node, SyllableVec &syls, int i, PathVec &fwdPaths)
{
	PathVec subPaths;
	if (findSubNode(node, syls.get(i), subPaths, 1)) {
		PathVec::iterator it = subPaths.begin();
		PathVec::iterator ite = subPaths.end();
		for (; it != ite; it++) {
			i += 1;
			if (i < syls.size()) {
				PathVec paths;
				if (findSubPath(path, (*it).front().m_TNode, syls, i, paths)) {
					return true;
				} else {
					return false;
				}
			} else {
				return true;
			}
		}
	} else {
		return false;
	}
}

bool
checkNumInPathSecondHalf(BranchPath &path, CheckPoint &cp, CheckPointVec &fwdPaths)
{
	SyllableVec syls = cp.getTransmits();
	return findSubPath(path, path.next(cp.m_TNode), syls, fwdPaths);
}

bool
checkNumInPath(BranchPath &path, TSyllable syllable, int num, BranchPathVec &fwdPaths)
{
	int stat = 0;
	CheckPointVec cpset = path.findNodesBySyllable(syllable);

	//search first half for num-1 cphooks
	while (true) {
		CheckPointVec cphooks;
		stat = getRepeaterStatus(path, cpset, num-1, cphooks);
		if (stat == 1) {
			goto OUT;
		} else if (stat == -1) {
			return false;
		}
		iterateRepeaters(path, cpset, num-1);
	}

OUT:
	if (cphooks.size() >= num) {
		return true;
	} else if (cphooks.size() == num -1) {
		CheckPoint &lastHook = cphooks.back();
		CheckPoint &lastChkp = cphooks.back();
		if (path.next(lastHook.m_TNode) == lastChkp.m_Start) {
			return checkNumInPathSecondHalf(path, lastHook, fwdPaths);
		}
	}
	return false;
}

bool
CheckPoint::checkNumInPaths(PathVec paths, TSyllable syllable, int num)
{
	if (num == 1)
		return true;

	PathVec::iterator it = paths.begin();
	PathVec::iterator ite = paths.end();
	for (; it != ite; it++) {
		PathVec fwdPaths;
		if (!checkNumInPath(path, syllable, num, fwdPaths)) {
			(*it).erase();
		} else {
			PathVec::iterator sit = subPaths.begin();
			PathVec::iterator site = subPaths.end();
			
			for (; sit != site; sit++) {
				paths.push_back(*sit);
			}
		}
	}

	return true;
}

