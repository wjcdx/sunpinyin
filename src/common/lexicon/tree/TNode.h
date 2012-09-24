#ifndef SUNPY_TREE_TNODE_H
#define SUNPY_TREE_TNODE_H

namespace TrieTreeModel {

    class TNode {
public:
        static CNodeList m_AllNodes;
public:
        CWordSet m_WordIdSet;
        CTrans m_Trans;
public:
		TNode()
		{
			m_AllNodes.push_back(this);
		}
    };
}

#endif

