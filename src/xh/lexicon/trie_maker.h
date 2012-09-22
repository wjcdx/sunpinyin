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

class CTrieMaker {
public:
    class TNode;
    class TWordInfo;

    union TWordId {
        unsigned int m_all;
        struct TAnony {                     //Some compiler do not support anonymous defaultly
        #ifdef WORDS_BIGENDIAN
            unsigned m_bHide    : 1;
            unsigned m_cost     : 5;
            unsigned m_csLevel  : 2;
            unsigned m_id       : WORD_ID_WIDTH;
        #else
            unsigned m_id       : WORD_ID_WIDTH;
            unsigned m_csLevel  : 2;
            unsigned m_cost     : 5;
            unsigned m_bHide    : 1;
        #endif
        } anony;

public:
        TWordId() : m_all(0) {
        }
        TWordId(const TWordId &b) : m_all(b.m_all) {
        }
        TWordId(unsigned id,
                unsigned cost = 0,
                unsigned hide = 0,
                unsigned cslvl = 0){
            anony.m_id = id;
            anony.m_cost = cost<31? cost: 31;
            anony.m_bHide = (hide) ? 1 : 0;
            anony.m_csLevel = cslvl<3? cslvl: 3;
        }

        bool operator<(const TWordId& b) const
        { return anony.m_id < b.anony.m_id; }

        bool operator==(const TWordId& b) const
        { return anony.m_id == b.anony.m_id; }

        operator unsigned int() const
        { return anony.m_id; }
    };

    typedef std::set<TWordId>               CWordSet;
    typedef std::vector<TWordInfo>          CWordVec;
    typedef std::map<unsigned, TNode*>      CTrans;
    typedef std::set<TNode*>                CNodeSet;
    typedef std::list<TNode*>               CNodeList;
    typedef std::vector<std::string>        CLexicon;

    class TWordInfo {
public:
        TWordId m_id;
        double m_cost;
        bool m_bSeen;

        TWordInfo(TWordId id = 0, double cost = 0.0, bool seen = false)
            : m_id(id), m_cost(cost), m_bSeen(seen){
            if (m_id.anony.m_bHide) {
                m_bSeen = false;
            }
            m_cost = cost + m_id.anony.m_cost;
        }

        bool operator<(const TWordInfo& b) const {
            double fa = (m_bSeen) ? (m_cost - 5000.0) : (m_cost);
            double fb = (b.m_bSeen) ? (b.m_cost - 5000.0) : (b.m_cost);
            return(fa < fb);
        }
    };

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

    typedef std::map<PNodeSet, TNode*>     CStateMap;

    class TNode {
public:
        static CNodeList m_AllNodes;
public:
        CWordSet m_WordIdSet;
        CTrans m_Trans;
public:
        TNode();
    };

protected:
    CStateMap m_StateMap;
    TNode m_RootNode;
    CLexicon m_Lexicon;

public:
    CTrieMaker();

    ~CTrieMaker() {} //forget this

    bool
    constructFromLexicon(const char* fileName);

    bool
    insertPair(const char* unit, TWordId wid);

    bool
    write(const char* fileName, CWordEvaluator* psrt, bool revert_endian);

    bool
    write(FILE *fp, CWordEvaluator* psrt, bool revert_endian);

protected:
    TNode*
    insertTransfer(TNode* pnode, unsigned s);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
