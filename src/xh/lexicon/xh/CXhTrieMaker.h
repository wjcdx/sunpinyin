// -*- mode: c++ -*-
#ifndef _SUNPINYIN_XHOCTRIE_MAKER_H__
#define _SUNPINYIN_XHOCTRIE_MAKER_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "common/lexicon/thread/TrieThreadModel.h"
#include "common/lexicon/tree/TrieTreeModel.h"
#include "common/lexicon/CWordEvaluator.h"

class CXhTrieMaker : public CTrieMaker {
public:
    CXhTrieMaker();

    ~CXhTrieMaker(); //forget this

    bool
    parseLine(char* buf,
              char* word_buf,
              unsigned& id,
              std::set<TUnitInfo>& unitset);

    bool
    constructFromLexicon(const char* fileName);

    bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    CTreeNode*
    insertTransfer(CTreeNode* pnode, unsigned s);

private:
    void
    findAllRealSubNodes(CTreeNode *pnode,
                        unsigned &syl,
                        const CTreeNodeSet& nodes);
    CTreeNode*
    addCombinedTransfers(CTreeNode *pnode,
                        unsigned s,
                        const CTreeNodeSet& nodes);
    void
    linkWordsTogether(CTreeNode *pnode);

    void
    clearUpWordIdsWithTooMuchStrokes(CTreeNode *pnode);
    void
    clearUpAllWordIds(CTreeNode *pnode);
    void
    clearUpWordIds(CTreeNode *pnode, bool skipPattern);

    void
    clearUpForeignWordIds(CTreeNode *pnode);
    void
    clearUpSinglePartialWordIds(CTreeNode *pnode, int stkno, bool ptnmet, bool bdrmet);
    
    void
    threadNonCompletedXh();
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
