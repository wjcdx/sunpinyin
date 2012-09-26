#ifndef SUNPY_TRIE_BRANCH_H
#define SUNPY_TRIE_BRANCH_H

struct CheckPoint {
	PathNode *m_Start;
	PathNode *m_PNode;
	CheckPoint(PathNode *node) {
		m_Start = m_PNode = node;
	}
};

typedef std::vector<CheckPoint> CheckPointVec;

#endif
