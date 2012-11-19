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
		: m_Now(NULL), m_bFullForwarded(false)
	{}

	Path(const Path &rhs) {
		operator=(rhs);
	}
	Path& operator=(const Path &rhs);

	void add(PathNode &node);
	void push_front(PathNode &node);

	PathNode *getNow() { return m_Now; }
	void setNow(PathNode *node) { m_Now = node; }

	bool getFullForwarded() { return m_bFullForwarded; }
	void setFullForwarded(bool fwd) { m_bFullForwarded = fwd; }

	void addPseudoHead();
	
	PathNodeList &getPathNodes() { return m_Nodes; }

	PathNode *next(PathNode *node);
	PathNode *next(TSyllable s);

	void forward();
	
	int getTransNum(TSyllable s);

	bool forward(TSyllable syllable, int num, PathList &paths);

	void
	printNodes();
	
	void
	printNextMap();

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
	bool m_bFullForwarded;

	CheckPointList cpset;
};

typedef std::list<Path> PathList;

class TrieBranch {
public:
	Path m_Path;
	bool m_bNewAdded;

	TrieBranch() : m_bNewAdded(true) {}

	Path &getPath() { return m_Path; }

	bool isNewAdded() { return m_bNewAdded; }
	void setNewAdded(bool fresh) { m_bNewAdded = fresh; }

	bool forward(TSyllable s, int num, PathList &paths) {
		return m_Path.forward(s, num, paths);
	}
	void addPathInfo(Path &path) {
		PathNodeList::iterator it = path.getPathNodes().begin();
		PathNodeList::iterator ite = path.getPathNodes().end();
		for (; it != ite; it++) {
			m_Path.add(*it);
		}
		m_Path.setFullForwarded(path.getFullForwarded());
	}
};

typedef std::list<TrieBranch> BranchList;

#endif

