#ifndef SUNPY_TREE_PYTNODE_H
#define SUNPY_TREE_PYTNODE_H

#include "common/lexicon/tree/CTreeNode.h"

namespace TrieTreeModel {
	class CPyTreeNode : public CTreeNode {
	public:
		bool m_bFullSyllableTransfer;

	public:
		CPyTreeNode()
			: m_bFullSyllableTransfer(false)
		{
		}
	};
}

#endif
