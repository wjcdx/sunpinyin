#ifndef SUNPY_PATH_NODE_H
#define SUNPY_PATH_NODE_H

struct PathNode;
typedef std::vector<PathNode> PathNodeVec;

struct PathNode {
	enum Flag {
		JUMPED,
		HISTORY,
		FUTURE,
		JUSTNOW,
		CHECKPOINT,
		END
	};
	TrieThreadModel::TTransmit *m_Trans;
	TrieThreadModel::TNode *m_TNode;
	unsigned flag;

public:
	bool isEnd() { return (flag == END); }
	bool transFrom(TSyllable s) { return (m_Trans == s); }
	TrieThreadModel::TNode *getTNode() { return m_TNode; }

	PathNodeVec getChildren(TSyllable syllable);
	bool PathNode::findNextSubNode(TSyllable syllable, PathVec &paths);
	bool PathNode::findAllSubNode(TSyllable syllable, int num, PathVec &paths);
};

#endif
