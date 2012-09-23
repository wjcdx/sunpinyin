#ifndef SUNPY_THREAD_TNODE_H
#define SUNPY_THREAD_TNODE_H

namespace TrieThreadModel {
    struct TNode {
    #ifdef WORDS_BIGENDIAN
        unsigned m_other      : 6;
        unsigned m_csLevel    : 2;
        unsigned m_nTransfer  : 12;
        unsigned m_nWordId    : 12;
    #else
        unsigned m_nWordId    : 12;
        unsigned m_nTransfer  : 12;
        unsigned m_csLevel    : 2;
        unsigned m_other      : 6;
    #endif

        static unsigned int size_for(unsigned int nTransfer,
                                     unsigned int nWordId)                    {
            return sizeof(TNode) + sizeof(TTransUnit) * nTransfer +
                   sizeof(TWordIdInfo) * nWordId;
        }

        TNode()
        { *((unsigned *) this) = 0; }

        bool hasChild(void) const
        { return(m_nTransfer > 1); }

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