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
		: m_Now(NULL), m_FullForwarded(false)
	{}

	Path(const Path &rhs) {
		operator=(rhs);
	}
	Path& operator=(const Path &rhs);

	void add(PathNode &node);
	void push_front(PathNode &node);

	PathNode *getNow() { return m_Now; }
	void setNow(PathNode *node) { m_Now = node; }

	void addPseudoHead();
	
	PathNodeList &getPathNodes() { return m_Nodes; }

	PathNode *next(PathNode *node);
	PathNode *next(TSyllable s);

	void forward();
	
	int getTransNum(TSyllable s);

	void setWordNode(PathNode &node) { m_NodeWithWord = node; }
	PathNode &getWordNode() { return m_NodeWithWord; }

	bool forward(TSyllable syllable, int num,
			bool forward, PathList &paths);

	void
	printNodes();
	
	void
	printNextMap();

	bool isFullForwarded() { return m_FullForwarded; }
	void setFullForwarded(bool fwd) { m_FullForwarded = fwd; }

private:	
	void add_first_node(PathNode &node) {
		m_Nodes.push_back(node);
		m_Now = &m_Nodes.back();
	}

public:
	bool checkNumInPath(TSyllable syllable, int num);

private:
	int getRepeaterStatus(int count, CheckPointList &cphooks);
	void forwardCheckPoint();
	int getSameRepNumber(CheckPoint &cp);
	void iterateRepeaters(int count);
	void findCheckPoints(TSyllable syllable);
	void labelPath(CheckPointList &cphooks);
	bool checkNumInPaths(TSyllable syllable, int num, PathList &paths);

private:
	PathNodeList m_Nodes;
	std::map<PathNode *, PathNode *> m_NextMap;
	PathNode *m_Now;
	PathNode m_NodeWithWord;
	bool m_FullForwarded;

	CheckPointList cpset;
};

typedef std::list<Path> PathList;

class TrieBranch {
public:
	Path m_Path;

	TrieBranch() {}

	Path &getPath() { return m_Path; }

	bool forward(TSyllable s, int num, bool forward, PathList &paths) {
		return m_Path.forward(s, num, forward, paths);
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

