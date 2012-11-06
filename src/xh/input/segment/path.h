#ifndef SUNPY_BRANCH_PATH_H
#define SUNPY_BRANCH_PATH_H

#include "pathnode.h"
#include "checkpoint.h"
#include <algorithm>
#include <list>

class Path {
private:
	PathNodeList m_Nodes;
	std::map<PathNode *, PathNode *> m_NextMap;
	PathNode *m_Now;

public:
	void add(PathNode &node) {
		PathNode *priv = &m_Nodes.back();
		m_Nodes.push_back(node);

		PathNode *n = &m_Nodes.back();
		m_NextMap[priv] = n;

		if (n->flag == PathNode::JUSTNOW) {
			m_Now->flag = PathNode::HISTORY;
			m_Now = n;
		}
	}
	
	void push_front(PathNode &node) {
		PathNode *next = &m_Nodes.front();
		m_Nodes.push_front(node);

		PathNode *n = &m_Nodes.front();
		m_NextMap[n] = next;

		if (n->flag == PathNode::JUSTNOW) {
			m_Now->flag = PathNode::FUTURE;
			m_Now = n;
		}
	}

	PathNode *getNow() {
		return m_Now;
	}
	
	PathNodeList &getPathNodes() {
		return m_Nodes;
	}

	PathNode *next(PathNode *node) {
		PathNodeList::iterator it = m_Nodes.begin();
		PathNodeList::iterator ite = m_Nodes.end();
		for (; it != ite; it++) {
			if (*it == *node) {
				return m_NextMap[&(*it)];
			}
		}
		return NULL;
	}

	PathNode *next(TSyllable s) {
		PathNode *nxt = m_NextMap[m_Now];
		if (nxt->transFrom(s)) {
			return nxt;
		}
		return NULL;
	}

	void forward() {
		PathNode *nxt = m_NextMap[m_Now];
		m_Now->flag = PathNode::HISTORY;
		nxt->flag = PathNode::JUSTNOW;
		m_Now = nxt;
	}
	
	int getTransNum(TSyllable s) {
		int c = 0;
		PathNodeList::iterator it = m_Nodes.begin();
		PathNodeList::iterator ite = m_Nodes.end();
		for (; it != ite; it++)
			if ((*it).transFrom(s))
				c++;
		return c;
	}

public:
	bool forward(TSyllable syllable, int num, bool pathInfoFull, PathList &paths);

private:
	CheckPointList cpset;
	int getRepeaterStatus(int count, CheckPointList &cphooks);
	void forwardCheckPoint();
	int getSameRepNumber(CheckPoint &cp);
	void iterateRepeaters(int count);
	void findCheckPoints(TSyllable syllable);
	void labelPath(CheckPointList &cphooks);
	bool checkNumInPath(TSyllable syllable, int num);
	bool checkNumInPaths(PathList paths, TSyllable syllable, int num);
};

typedef std::list<Path> PathList;

class TrieBranch {
public:
	Path m_Path;
	bool newAdded;

	bool forward(TSyllable s, int num, bool numMet, PathList &paths) {
		return m_Path.forward(s, num, numMet, paths);
	}
	void addPathInfo(Path &path) {
		PathNodeList::iterator it = path.getPathNodes().begin();
		PathNodeList::iterator ite = path.getPathNodes().end();
		for (; it != ite; it++) {
			m_Path.add(*it);
		}
	}
};

typedef std::list<TrieBranch> BranchList;

#endif

