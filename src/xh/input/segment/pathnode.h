#ifndef SUNPY_PATH_NODE_H
#define SUNPY_PATH_NODE_H

#include "TrieThreadModel.h"

struct PathNode;
typedef std::vector<PathNode> PathNodeVec;

struct Path;
typedef std::vector<Path> PathVec;

enum PathNodeFlag {
	JUMPED,
	HISTORY,
	FUTURE,
	JUSTNOW,
	CHECKPOINT,
	END
};

struct PathNode {
	TrieThreadModel::TTransUnit *m_Trans;
	TrieThreadModel::TThreadNode *m_TNode;
	unsigned flag;
	
public:
	PathNode(PathNodeFlag f) : flag(f)
	{}

	bool isEnd() { return (flag == END); }
	bool transFrom(TSyllable s) { return (m_Trans->m_Unit == s); }
	TrieThreadModel::TThreadNode *getTNode() { return m_TNode; }

	PathNodeVec getChildren(TSyllable syllable);
	bool findNextSubNode(TSyllable syllable, PathVec &paths);
	bool findAllSubNode(TSyllable syllable, int num, PathVec &paths);
};

#endif
