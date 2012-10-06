// -*- mode: c++ -*-
#ifndef _SUNPINYIN_XHTRIE_MAKER_H__
#define _SUNPINYIN_XHTRIE_MAKER_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "TrieThreadModel.h"
#include "TrieTreeModel.h"
#include "CWordEvaluator.h"

class CXhTrieMaker : public CTrieMaker {
public:
    CXhTrieMaker();

    ~CXhTrieMaker(); //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    CTreeNode*
    insertTransfer(CTreeNode* pnode, unsigned s);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
