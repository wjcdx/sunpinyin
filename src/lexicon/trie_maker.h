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
#include "CWordEvaluator.h"
using TrieTreeModel;

class CTrieMaker {
public:
    typedef std::set<TWordId>               CWordSet;
    typedef std::vector<TWordInfo>          CWordVec;
    typedef std::map<unsigned, TNode*>      CTrans;
    typedef std::set<TNode*>                CNodeSet;
    typedef std::list<TNode*>               CNodeList;
    typedef std::vector<std::string>        CLexicon;

    typedef std::map<PNodeSet, TNode*>     CStateMap;

protected:
    CStateMap m_StateMap;
    TNode m_RootNode;
    CLexicon m_Lexicon;
    CUnitData m_UnitData;

public:
    CTrieMaker();

    virtual ~CTrieMaker() {} //forget this

    virtual bool
    constructFromLexicon(const char* fileName);

    virtual bool
    insertPair(const char* unit, TWordId wid);

    virtual bool
    write(const char* fileName, CWordEvaluator* psrt, bool revert_endian);

    virtual bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    TNode*
    insertTransfer(TNode* pnode, unsigned s);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
