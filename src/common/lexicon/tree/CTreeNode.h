#ifndef SUNPY_TREE_TNODE_H
#define SUNPY_TREE_TNODE_H

#include <list>
#include <set>
#include <map>

#include "TTreeWordId.h"

namespace TrieTreeModel {

	class CTreeNode;

	typedef std::set<CTreeNode*>                CTreeNodeSet;
	typedef std::list<CTreeNode*>               CTreeNodeList;
	typedef std::map<unsigned, CTreeNode*>      CTrans;

    class CTreeNode {
public:
        static CTreeNodeList m_AllNodes;
        CTreeWordSet m_WordIdSet;
        CTrans m_Trans;
		unsigned m_nMaxStroke;

public:
		CTreeNode()
		{
			m_nMaxStroke = 0;
			m_AllNodes.push_back(this);
		}
    };
}

#endif

