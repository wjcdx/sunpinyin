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

public:
		bool m_bExpanded;
		bool m_bPesudo;
		CTreeNodeSet m_cmbNodes;
        CTreeWordSet m_WordIdSet;
        CTrans m_Trans;
		unsigned m_nMaxStroke;
		bool m_bOwnWord;
		unsigned m_nStrokeNumber;

public:
		CTreeNode()
			: m_bExpanded(false), m_bPesudo(false), m_nMaxStroke(0), m_bOwnWord(false),
			  m_nStrokeNumber(0)
		{
			m_AllNodes.push_back(this);
		}
    };
}

#endif

