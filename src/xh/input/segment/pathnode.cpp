#include "pathnode.h" 
#include "path.h" 
#include "checkpoint.h" 

PathNodeList
PathNode::getChildren(TSyllable syllable)
{
	PathNodeList nodes;
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
		Path path;
		PathNode node(PathNode::END);
		path.push_front(node);
		paths.push_back(path);
		return true;
	}

	PathNodeList::iterator nit = children.begin();
	PathNodeList::iterator nite = children.end();
	for (; nit != nite; nit++) {
		if ((*nit).transFrom(syllable)) {
			(*nit).flag = PathNode::FUTURE;
			num -= 1;
		} else {
			(*nit).flag = PathNode::CHECKPOINT;
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

