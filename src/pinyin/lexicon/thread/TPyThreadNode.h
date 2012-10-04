#ifndef SUNPY_THREAD_PYTNODE_H
#define SUNPY_THREAD_PYTNODE_H

#include "TThreadNode.h"

namespace TrieThreadModel {

    struct TPyThreadNode : public TThreadNode {
#if 0
    #ifdef WORDS_BIGENDIAN
        unsigned m_other      : 5;
        unsigned m_bFullSyllableTransfer : 1;
        unsigned m_csLevel    : 2;
        unsigned m_nTransfer  : 12;
        unsigned m_nWordId    : 12;
    #else
        unsigned m_nWordId    : 12;
        unsigned m_nTransfer  : 12;
        unsigned m_csLevel    : 2;
        unsigned m_bFullSyllableTransfer : 1;
        unsigned m_other      : 5;
    #endif
#endif
		unsigned getOther() {
    #ifdef WORDS_BIGENDIAN
			return (m_other >> 1);
	#else
			return (m_other & 0x1F);
	#endif
		}
		void setOther(unsigned v) {
    #ifdef WORDS_BIGENDIAN
			m_other = ((m_other & 0x01) | (v << 1));
	#else
			m_other = ((m_other & 0x20) | (v));
	#endif
		}
		unsigned getFullSyllableTransfer() {
    #ifdef WORDS_BIGENDIAN
			return (m_other & 0x01);
	#else
			return (m_other >> 5);
	#endif
		}
		void setFullSyllableTransfer(unsigned v){
    #ifdef WORDS_BIGENDIAN
			m_other = ((m_other & 0xFE) | (v));
	#else
			m_other = ((m_other & 0xDF) | (v << 5));
	#endif
		}
    };

}

#endif
