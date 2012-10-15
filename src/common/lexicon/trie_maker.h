// -*- mode: c++ -*-
#ifndef _SUNPINYIN_TRIE_MAKER_H__
#define _SUNPINYIN_TRIE_MAKER_H__

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "trie.h"
#include "CUnitData.h"
#include "tree/TrieTreeModel.h"
#include "CWordEvaluator.h"
#include "thread/TUnitInfo.h"

using namespace TrieTreeModel;

class CTrieMaker {
public:
    typedef std::vector<std::string>        CLexicon;
    typedef std::map<PNodeSet, CTreeNode*>  CStateMap;

protected:
    CStateMap m_StateMap;
    CTreeNode *m_pRootNode;
    CLexicon m_Lexicon;

public:
    CTrieMaker();

    virtual ~CTrieMaker() {} //forget this

    virtual bool
    constructFromLexicon(const char* fileName);
    
    bool
    insertPair(const char* ustr, TTreeWordId wid);
    
    void
    parseUnit(const char *ustr, std::vector<TUnit> &ret);
    
    unsigned
    getPureGBEncoding(const char* utf8str);

    bool
    parseLine(char* buf,
              char* word_buf,
              unsigned& id,
              std::set<TUnitInfo>& unitset);

    virtual bool
    write(const char* fileName, CWordEvaluator* psrt, bool revert_endian);

    virtual bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    virtual CTreeNode*
    insertTransfer(CTreeNode* pnode, unsigned s);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
