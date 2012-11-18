#ifndef SUNPY_THREAD_TNODE_H
#define SUNPY_THREAD_TNODE_H

#include "TTransUnit.h"
#include "TWordIdInfo.h"

namespace TrieThreadModel {
    struct TThreadNode {
    #ifdef WORDS_BIGENDIAN
        unsigned m_nMaxStroke : 3;
        unsigned m_bOwnWord   : 1;
        unsigned m_csLevel    : 2;
        unsigned m_nTransfer  : 13;
        unsigned m_nWordId    : 13;
    #else
        unsigned m_nWordId    : 13;
        unsigned m_nTransfer  : 13;
        unsigned m_csLevel    : 2;
        unsigned m_bOwnWord   : 1;
        unsigned m_nMaxStroke : 3;
    #endif

        static unsigned int size_for(unsigned int nTransfer,
                                     unsigned int nWordId)                    {
            return sizeof(TThreadNode) + sizeof(TTransUnit) * nTransfer +
                   sizeof(TWordIdInfo) * nWordId;
        }

        TThreadNode()
        { *((unsigned *) this) = 0; }

        bool hasChild(void) const
        { return(m_nTransfer > 1); }
        
		bool hasItsOwnWord(void) const
        { return (m_bOwnWord == 1); }

        const TTransUnit*getTrans() const
        { return (TTransUnit *) (this + 1); }

        const TWordIdInfo*getWordIdPtr() const
        { return (TWordIdInfo *) (((char *) (this +
                                             1)) + sizeof(TTransUnit) *
                                  m_nTransfer); }

        unsigned int transfer(unsigned s) const {
            unsigned int b = 0, e = m_nTransfer;
            const TTransUnit* ptrans = getTrans();
            while (b < e) {
                int m = b + (e - b) / 2;
                if (ptrans[m].m_Unit == s)
                    return ptrans[m].m_Offset;
                if (ptrans[m].m_Unit < s)
                    b = m + 1;
                else
                    e = m;
            }
            return 0;
        }
    };
}

#endif
