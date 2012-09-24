#ifndef SUNPY_TRIE_BRANCH_H
#define SUNPY_TRIE_BRANCH_H

struct CheckPoint;
typedef std::vector<CheckPoint> CheckPointVec;

struct CheckPoint {
	TrieThreadModel::TNode *m_Start;
	TrieThreadModel::TNode *m_TNode;

public:
	void historify() { footprints.push_back(m_TNode); }
	bool forward(TSyllable &syllable, CheckPointVec cpset);
	void setTNode(TNode *n) { m_TNode = n; }
	bool findTNode(TNode *node, TSyllable syllable, BranchPath &pathes);
};

struct BranchPath {
private:
	std::map<TNode *, TNode *> m_NextMap;

public:
	void add(TNode *n) {
		TNode *priv = m_NextMap.back().second();
		m_NextMap[priv] = n;
	}

	TNode *next(TNode *n) {
		return m_NextMap[n];
	}
};

typedef std::vector<BranchPath> PathVec;

typedef BranchPath SearchPath;

struct TrieBranch {
	CheckPoints checkPoint;
	BranchPath m_Path;
	bool newAdded;
};

typedef std::vector<TrieBranch> BranchVec;

#endif
