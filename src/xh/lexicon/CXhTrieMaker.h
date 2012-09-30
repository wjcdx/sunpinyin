// -*- mode: c++ -*-
#ifndef _SUNPINYIN_PYTRIE_GEN_H__
#define _SUNPINYIN_PYTRIE_GEN_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "CXhTrie.h"
#include "TThreadNode.h"
#include "CWordEvaluator.h"

class CXhTrieMaker : CXhTrieMaker {
public:
    class TThreadNode;
    class TWordInfo;

    typedef std::vector<std::string>                CLexicon;
    typedef std::map<PNodeSet, TThreadNode*>        CStateMap;

protected:
    CStateMap m_StateMap;
    TThreadNode m_RootNode;
    CLexicon m_Lexicon;

public:
    CXhTrieMaker();

    ~CXhTrieMaker() {} //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    insertPair(const char* unit, TWordId wid);

    bool
    write(const char* fileName, CWordEvaluator* psrt, bool revert_endian);

    bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    TThreadNode*
    insertTransfer(TThreadNode* pnode, unsigned s);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
