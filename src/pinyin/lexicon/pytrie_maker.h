// -*- mode: c++ -*-
#ifndef _SUNPINYIN_PYTRIE_MAKER_H__
#define _SUNPINYIN_PYTRIE_MAKER_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "common/lexicon/trie.h"
#include "common/lexicon/trie_maker.h"
#include "common/lexicon/tree/TrieTreeModel.h"
#include "common/lexicon/CWordEvaluator.h"
#include "tree/CPyTreeNode.h"

class CPyTrieMaker : public CTrieMaker {
public:
    CPyTrieMaker();

    ~CPyTrieMaker(); //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    threadNonCompletePinyin(void);

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
