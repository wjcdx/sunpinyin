#ifndef SUNPY_TREE_PNODESET_H
#define SUNPY_TREE_PNODESET_H

namespace TrieTreeModel {

    class PNodeSet {
public:
        PNodeSet(const CNodeSet *pns) : m_pns(pns) { }

        PNodeSet(const PNodeSet& another) : m_pns(another.m_pns) { }

        const CNodeSet*operator->(void) { return m_pns; }

        const CNodeSet&operator*(void) { return *m_pns; }

        bool
        operator<(const PNodeSet& another) const;

        bool
        operator==(const PNodeSet& another) const;

protected:
        const CNodeSet *    m_pns;
    };
}

#endif
