#ifndef SUNPY_PATH_NODE_H
#define SUNPY_PATH_NODE_H

#include "common/lexicon/thread/TrieThreadModel.h"
#include "pinyin/input/syllable.h"
#include <list>

class PathNode;
typedef std::list<PathNode> PathNodeList;

class Path;
typedef std::list<Path> PathList;

class PathNode {

public:
	enum PathNodeFlag {
		JUMPED,
		HISTORY,
		FUTURE,
		JUSTNOW,
		CHECKPOINT
	};

	TrieThreadModel::TTransUnit *m_Trans;
	TrieThreadModel::TThreadNode *m_TNode;
	PathNodeFlag flag;
	
	PathNode()
		: m_Trans(NULL), m_TNode(NULL), flag(FUTURE)
	{}
	PathNode(PathNodeFlag f)
		: m_Trans(NULL), m_TNode(NULL), flag(f)
	{}
	
	PathNode(TrieThreadModel::TTransUnit *trans,
			TrieThreadModel::TThreadNode *node,
			PathNodeFlag f = FUTURE)
		: m_Trans(trans), m_TNode(node), flag(f)
	{}

	bool operator==(PathNode &node) {
		return (m_Trans == node.m_Trans
				&& m_TNode == node.m_TNode);
	}

	void setFlag(PathNodeFlag f) { flag = f; }
	bool isNow() { return flag == PathNode::JUSTNOW; }

	bool transFrom(TSyllable s) { return (m_Trans->m_Unit == s); }
	TrieThreadModel::TThreadNode *getTNode() { return m_TNode; }

	void print();
	bool getChildren(TSyllable syllable, PathNodeList &children);
	bool findNextSubNode(TSyllable syllable, PathList &paths);
	bool findAllSubNode(TSyllable syllable, int num, PathList &paths);
};

#endif
