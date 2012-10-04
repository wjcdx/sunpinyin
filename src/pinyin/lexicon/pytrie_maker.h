// -*- mode: c++ -*-
#ifndef _SUNPINYIN_PYTRIE_MAKER_H__
#define _SUNPINYIN_PYTRIE_MAKER_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "trie.h"
#include "trie_maker.h"
#include "TrieTreeModel.h"
#include "CWordEvaluator.h"
#include "CPyTreeNode.h"

class CPyTrieMaker : public CTrieMaker {
public:
    CPyTrieMaker();

    ~CPyTrieMaker(); //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    threadNonCompletePinyin(void);

    bool
    write(const char* fileName, CWordEvaluator* psrt, bool revert_endian);

    bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    //override
    CTreeNode*
    insertTransfer(CTreeNode* pnode, unsigned s);

    CPyTreeNode*
    addCombinedTransfers(CPyTreeNode *pnode, unsigned s, const CTreeNodeSet& nodes);

    void
    combineInitialTrans(CPyTreeNode *pnode);

    void
    expandCombinedNode(CPyTreeNode *pnode);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
