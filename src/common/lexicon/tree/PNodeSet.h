#ifndef SUNPY_TREE_PNODESET_H
#define SUNPY_TREE_PNODESET_H

#include "CTreeNode.h"

namespace TrieTreeModel {
    class PNodeSet {
public:
        PNodeSet(const CTreeNodeSet *pns) : m_pns(pns) { }

        PNodeSet(const PNodeSet& another) : m_pns(another.m_pns) { }

        const CTreeNodeSet*operator->(void) { return m_pns; }

        const CTreeNodeSet&operator*(void) { return *m_pns; }

        bool
        operator<(const PNodeSet& another) const;

        bool
        operator==(const PNodeSet& another) const;

protected:
        const CTreeNodeSet *    m_pns;
    };
}

#endif
