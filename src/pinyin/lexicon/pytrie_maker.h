// -*- mode: c++ -*-
#ifndef _SUNPINYIN_PYTRIE_GEN_H__
#define _SUNPINYIN_PYTRIE_GEN_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "trie.h"
#include "CPyTrie.h"

class CPyTrieMaker : CTrieMaker  {
public:
    CPinyinTrieMaker();

    ~CPinyinTrieMaker() {} //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    threadNonCompletePinyin(void);

    bool
    write(const char* fileName, CWordEvaluator* psrt, bool revert_endian);

    bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    TNode*
    insertTransfer(TNode* pnode, unsigned s);

    TNode*
    addCombinedTransfers(TNode *pnode, unsigned s, const CNodeSet& nodes);

    void
    combineInitialTrans(TNode *pnode);

    void
    expandCombinedNode(TNode *pnode);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
