#ifndef SUNPY_TREE_PYTNODE_H
#define SUNPY_TREE_PYTNODE_H

#include "common/lexicon/tree/CTreeNode.h"

namespace TrieTreeModel {
	class CPyTreeNode : public CTreeNode {
	public:
		bool m_bExpanded;
		bool m_bFullSyllableTransfer;
		CTreeNodeSet m_cmbNodes;

	public:
		CPyTreeNode()
			: m_bExpanded(false), m_bFullSyllableTransfer(false)
		{
		}
	};
}

#endif
