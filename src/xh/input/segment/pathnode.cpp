#include "pathnode.h" 
#include "checkpoint.h" 

PathNodeVec
getChildren(TSyllable syllable)
{
	PathNodeVec nodes;
	return nodes;
}

bool
PathNode::findNextSubNode(TSyllable syllable, PathVec &paths)
{
	PathNodeVec children = this.getChildren(syllable);
	if (children.empty()) {
		return false;
	}

	PathNodeVec::iterator nit = children.begin();
	PathNodeVec::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if ((*nit).transFrom(syllable)) {
			(*nit).flag = JUSTNOW;

			Path path;
			path.push_front(nit);
			paths.push_back(path);
		} else {
			(*nit).flag = JUMPED;
			
			bool suc = false;
			PathVec subPaths;
			suc = (*nit).findNextSubNode(syllable, subPaths);
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
PathNode::findAllSubNode(TSyllable syllable, int num, PathVec &paths)
{
	PathNodeVec children = this.getChildren(syllable);
	if (children.empty()) {
		Path path;
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
		suc = (*nit).findAllSubNode(syllable, num, subPaths);
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

