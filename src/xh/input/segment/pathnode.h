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
	unsigned m_Level;
	bool m_FirstInPartial;
	
	PathNode()
		: m_Trans(NULL), m_TNode(NULL), flag(FUTURE), m_Level(0), m_FirstInPartial(false)
	{}
	PathNode(PathNodeFlag f)
		: m_Trans(NULL), m_TNode(NULL), flag(f), m_Level(0), m_FirstInPartial(false)
	{}
	
	PathNode(TrieThreadModel::TTransUnit *trans,
			TrieThreadModel::TThreadNode *node,
			unsigned level = 0,
			bool first = false,
			PathNodeFlag f = FUTURE)
		: m_Trans(trans), m_TNode(node), flag(f), m_Level(level), m_FirstInPartial(first)
	{}

	bool operator==(PathNode &node) {
		return (m_Trans == node.m_Trans
				&& m_TNode == node.m_TNode);
	}

	void setFlag(PathNodeFlag f) { flag = f; }
	bool isNow() { return flag == PathNode::JUSTNOW; }

	bool transFrom(TSyllable s) { return (m_Trans->m_Unit == s); }
	unsigned getTransUnit() { return m_Trans->m_Unit; }
	bool isTransSameAs(PathNode *node) {
		return node != NULL && (getTransUnit() == node->getTransUnit());
	}

	unsigned getLevel() { return m_Level; }
	void setLevel(unsigned n) { m_Level = n; }

	bool isFirstInPartial() { return m_FirstInPartial; }
	void setFirstInPartial(bool first) { m_FirstInPartial = first; }


	bool isMostPopularStrokeGe(unsigned n);


	TrieThreadModel::TThreadNode *getTNode() { return m_TNode; }

	bool findNextSubNode(TSyllable syllable, PathList &paths);

	bool findAllSubNode(TSyllable syllable, int num, PathList &paths, Path &path);

private:
	void print();

	bool getChildrenFromPesudoTNode(TrieThreadModel::TTransUnit *pTrans,
			TrieThreadModel::TThreadNode *psuNode, PathNodeList &children);

	bool getChildren(PathNodeList &children, TSyllable syllable);

	bool findFirstSubNodeInPartial(TSyllable syllable, PathList &paths);

	bool tryNextPartial(TSyllable syllable, PathList &paths);
};

#endif
