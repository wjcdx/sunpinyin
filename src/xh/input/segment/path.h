#ifndef SUNPY_BRANCH_PATH_H
#define SUNPY_BRANCH_PATH_H

#include "pathnode.h"
#include "checkpoint.h"

struct Path {
private:
	PathNodeVec m_Nodes;
	std::map<PathNode *, PathNode *> m_NextMap;
	PathNode *m_Now;

public:
	void add(PathNode &node) {
		PathNode *priv = (*(m_NextMap.end())).second;
		m_NextMap[priv] = &node;
		m_Nodes.push_back(node);
		if (node.flag == JUSTNOW) {
			m_Now->flag = HISTORY;
			m_Now = &node;
		}
	}

	PathNode &getNow() {
		return *m_Now;
	}
	
	PathNodeVec &getPathNodes() {
		return m_Nodes;
	}

	PathNode *next(PathNode &n) {
		return m_NextMap[&n];
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
		m_Now->flag = HISTORY;
		nxt->flag = JUSTNOW;
		m_Now = nxt;
	}
	
	int getTransNum(TSyllable s) {
		int c = 0;
		PathNodeVec::iterator it = m_Nodes.begin();
		PathNodeVec::iterator ite = m_Nodes.end();
		for (; it != ite; it++)
			if ((*it).transFrom(s))
				c++;
		return c;
	}

public:
	bool forward(TSyllable syllable, int num, bool pathInfoFull, PathVec &paths);

private:
	CheckPointVec cpset;
	int getRepeaterStatus(int count, CheckPointVec &cphooks);
	void forwardCheckPoint();
	int getSameRepNumber(CheckPoint &cp);
	void iterateRepeaters(int count);
	void findCheckPoints(TSyllable syllable);
	void labelPath();
	bool checkNumInPath(TSyllable syllable, int num);
	bool checkNumInPaths(PathVec paths, TSyllable syllable, int num);
};

typedef std::vector<Path> PathVec;

struct TrieBranch {
	Path m_Path;
	bool newAdded;

public:
	bool forward(TSyllable s, int num, bool numMet, PathVec &paths) {
		return m_Path.forward(s, num, numMet, paths);
	}
	void addPathInfo(Path &path) {
		PathNodeVec::iterator it = path.getPathNodes().begin();
		PathNodeVec::iterator ite = path.getPathNodes().end();
		for (; it != ite; it++) {
			m_Path.add(*it);
		}
	}
};

typedef std::vector<TrieBranch> BranchVec;

#endif

