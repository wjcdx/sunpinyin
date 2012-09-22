#ifndef SUNPY_TREE_PYTNODE_H
#define SUNPY_TREE_PYTNODE_H

namespace PYTrieTreeModel {

class TNode : TrieTreeModel::TNode {
public:
	bool m_bExpanded;
	bool m_bFullSyllableTransfer;
	CNodeSet m_cmbNodes;

public:
	TNode()
		: m_bExpanded(false), m_bFullSyllableTransfer(false)
	{
		m_AllNodes.push_back(this);
	}
};

}

#endif
