// -*- mode: c++ -*-
#ifndef _SUNPINYIN_PYTRIE_GEN_H__
#define _SUNPINYIN_PYTRIE_GEN_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "pytrie.h"

class CPinyinTrieMaker : CTrieMaker  {
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

public:
    CPinyinTrieMaker();

    ~CPinyinTrieMaker() {} //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    insertFullPinyinPair(const char* pinyin, TWordId wid);

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
