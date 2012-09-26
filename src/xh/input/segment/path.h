#ifndef SUNPY_BRANCH_PATH_H
#define SUNPY_BRANCH_PATH_H

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
	bool forward(TSyllable &syllable, PathNodeVec cpset);
	void setTNode(TNode *n) { m_TNode = n; }
	bool findTNode(TNode *node, TSyllable syllable, BranchPath &pathes);
	bool isEnd() { return (flag == END); }
	bool transFrom(TSyllable s) { return (m_Trans == s); }
};

struct BranchPath {
private:
	PathNodeVec m_Nodes;
	std::map<PathNode *, PathNode *> m_NextMap;
	PathNode &m_Now;

public:
	void add(PathNode &cp) {
		PathNode *priv = m_NextMap.back().second();
		m_NextMap[priv] = n;
		m_Nodes.push_back(cp);
		if (cp.flag == JUSTNOW) {
			m_Now.flag = HISTORY;
			m_Now = cp;
		}
	}

	PathNode &getNow() {
		return m_Now;
	}

	PathNode &next(PathNode &n) {
		return m_NextMap[n];
	}

	PathNode &next(TSyllable s) {
		PathNode &nxt = m_NextMap[m_Now];
		if (nxt.transFrom(s)) {
			return nxt;
		}
		return NULL;
	}

	void forward() {
		PathNode &nxt = m_NextMap[m_Now];
		m_Now.flag = HISTORY;
		nxt.flag = JUSTNOW;
		m_Now = nxt;
	}
	
	int getTransNum(TSyllable s) {
		int c = 0;
		PathNodeVec::iterator it = m_Nodes.begin();
		PathNodeVec::iterator ite = m_Nodes.end();
		if ((*it).transFrom(s))
			c++;
		return c;
	}
};

typedef std::vector<BranchPath> PathVec;

struct TrieBranch {
	BranchPath m_Path;
	bool newAdded;
public:
	void addPathInfo(BranchPath &path) {
		PathNodeVec::iterator it = path.m_Nodes.begin();
		PathNodeVec::iterator ite = path.m_Nodes.end();
		m_Path.add(*it);
	}
};

typedef std::vector<TrieBranch> BranchVec;

#endif

