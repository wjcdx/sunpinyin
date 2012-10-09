// -*- mode: c++ -*-
#ifndef __SUNPINYIN_TRIE_WRITER_H__
#define __SUNPINYIN_TRIE_WRITER_H__

#include "writer.h"
#include "trie.h"

template <>
class OtherEndian<TThreadNode>
{
    struct TThreadNode_BE {
        unsigned m_other      : 6;
        unsigned m_csLevel    : 2;
        unsigned m_nTransfer  : 12;
        unsigned m_nWordId    : 12;
    };

    struct TThreadNode_LE {
        unsigned m_nWordId    : 12;
        unsigned m_nTransfer  : 12;
        unsigned m_csLevel    : 2;
        unsigned m_other      : 6;
    };
public:
    DEFINE_OTHER_TYPE(TThreadNode);

    static TargetType create(const TThreadNode& from){
        TargetType to;
        to.m_nTransfer = from.m_nTransfer;
        to.m_nWordId = from.m_nWordId;
        to.m_csLevel = from.m_csLevel;
        // we don't care about m_other though
        to.m_other = from.m_other;
        return to;
    }
};

template<>
class OtherEndian<TUnit>
{
public:
    typedef TUnit TargetType;

    static TargetType create(const TUnit &from){
        TargetType to;
        to.m_Unit = from.m_Unit;
        return to;
    }
};

template <>
class OtherEndian<TWordIdInfo>
{
    struct TWordIdInfo_BE {
        unsigned m_bSeen    : 1;
        unsigned m_cost     : 5;
        unsigned m_csLevel  : 2;
        unsigned m_id       : WORD_ID_WIDTH;
    };

    struct TWordIdInfo_LE {
        unsigned m_id       : WORD_ID_WIDTH;
        unsigned m_csLevel  : 2;
        unsigned m_cost     : 5;
        unsigned m_bSeen    : 1;
    };

public:
    DEFINE_OTHER_TYPE(TWordIdInfo);

    static TargetType create(const TWordIdInfo& from){
        TargetType to;
        to.m_id = from.m_id;
        to.m_csLevel = from.m_csLevel;
        to.m_bSeen = from.m_bSeen;
        to.m_cost = from.m_cost;
        return to;
    }
};

template <>
bool revert_write<TTransUnit> (const TTransUnit& t,
                                            FILE *fp);

#endif //__SUNPINYIN_PYTRIE_WRITER_H__

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
