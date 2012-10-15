#ifndef SUNPY_PATH_NODE_H
#define SUNPY_PATH_NODE_H

#include "common/lexicon/thread/TrieThreadModel.h"
#include "pinyin/input/syllable.h"
#include <list>

struct PathNode;
typedef std::list<PathNode> PathNodeList;

struct Path;
typedef std::list<Path> PathList;

struct PathNode {

	enum PathNodeFlag {
		JUMPED,
		HISTORY,
		FUTURE,
		JUSTNOW,
		CHECKPOINT,
		END
	};

	TrieThreadModel::TTransUnit *m_Trans;
	TrieThreadModel::TThreadNode *m_TNode;
	unsigned flag;
	
public:
	PathNode() {}
	PathNode(PathNodeFlag f) : flag(f)
	{}

	bool operator==(PathNode &node) {
		return (m_Trans == node.m_Trans
				&& m_TNode == node.m_TNode);
	}

	bool isEnd() { return (flag == END); }
	bool transFrom(TSyllable s) { return (m_Trans->m_Unit == s); }
	TrieThreadModel::TThreadNode *getTNode() { return m_TNode; }

	PathNodeList getChildren(TSyllable syllable);
	bool findNextSubNode(TSyllable syllable, PathList &paths);
	bool findAllSubNode(TSyllable syllable, int num, PathList &paths);
};

#endif
