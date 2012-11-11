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
	PathNode m_NodeWithWord;

public:
	Path(){};

	Path(PathNode &node)
	{
		add_first_node(node);
	}

	void add(PathNode &node) {
		if (m_Nodes.empty()) {
			add_first_node(node);
			return;
		}

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
		if (m_Nodes.empty()) {
			add_first_node(node);
			return;
		}
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

	void initNodeNow() {
		if (m_Nodes.empty())
			return;
		m_Now = &m_Nodes.front();
	}
	
	PathNodeList &getPathNodes() {
		return m_Nodes;
	}

	PathNode *next(PathNode *node) {
		return m_NextMap[node];
	}

	PathNode *next(TSyllable s) {
		PathNode *nxt = m_NextMap[m_Now];
		if (nxt && nxt->transFrom(s)) {
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

	void setWordNode(PathNode &node) {
		m_NodeWithWord = node;
	}
	
	PathNode &getWordNode() {
		return m_NodeWithWord;
	}

public:
	bool forward(TSyllable syllable, int num, bool pathInfoFull, PathList &paths);

private:

	void add_first_node(PathNode &node)
	{
		m_Nodes.push_back(node);
		m_Now = &m_Nodes.back();
	}

	CheckPointList cpset;
	int getRepeaterStatus(int count, CheckPointList &cphooks);
	void forwardCheckPoint();
	int getSameRepNumber(CheckPoint &cp);
	void iterateRepeaters(int count);
	void findCheckPoints(TSyllable syllable);
	void labelPath(CheckPointList &cphooks);
	bool checkNumInPath(TSyllable syllable, int num);
	bool checkNumInPaths(TSyllable syllable, int num, PathList &paths);
};

typedef std::list<Path> PathList;

class TrieBranch {
public:
	Path m_Path;
	bool newAdded;

	Path &getPath() {
		return m_Path;
	}

	bool forward(TSyllable s, int num, bool numMet, PathList &paths) {
		return m_Path.forward(s, num, numMet, paths);
	}
	void addPathInfo(Path &path) {
		PathNodeList::iterator it = path.getPathNodes().begin();
		PathNodeList::iterator ite = path.getPathNodes().end();
		for (; it != ite; it++) {
			m_Path.add(*it);
		}
		m_Path.setWordNode(path.getWordNode());
	}
};

typedef std::list<TrieBranch> BranchList;

#endif

