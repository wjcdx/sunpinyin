#ifndef SUNPY_BRANCH_PATH_H
#define SUNPY_BRANCH_PATH_H

#include "portability.h"
#include "pathnode.h"
#include "checkpoint.h"
#include <algorithm>
#include <list>

class Path {
public:
	Path()
		: m_Now(NULL)
	{}

	Path(const Path &rhs) {
		operator=(rhs);
	}
	Path& operator=(const Path &rhs);

	void add(PathNode &node);
	void push_front(PathNode &node);

	PathNode *getNow() { return m_Now; }
	void resetNowNode() {
		if (m_Nodes.empty())
			return;
		m_Now = &m_Nodes.front();
	}
	
	PathNodeList &getPathNodes() { return m_Nodes; }

	PathNode *next(PathNode *node);
	PathNode *next(TSyllable s);

	void forward();
	
	int getTransNum(TSyllable s);

	void setWordNode(PathNode &node) { m_NodeWithWord = node; }
	PathNode &getWordNode() { return m_NodeWithWord; }

public:
	bool forward(TSyllable syllable, int num, bool pathInfoFull, PathList &paths);

	void
	printNodes();
	
	void
	printNextMap();

private:	
	void add_first_node(PathNode &node) {
		m_Nodes.push_back(node);
		m_Now = &m_Nodes.back();
	}

private:
	PathNodeList m_Nodes;
	std::map<PathNode *, PathNode *> m_NextMap;
	PathNode *m_Now;
	PathNode m_NodeWithWord;

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

	TrieBranch()
	 : newAdded(false)
	{}

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

