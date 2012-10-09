#ifndef SUNPY_LATTICE_H
#define SUNPY_LATTICE_H

#include <string>
#include <map>

#include "candidate.h"
#include "lexicon_states.h"
#include "lattice_states.h"


class CLatticeFrame {
    friend class CIMIContext;
    friend class CLatticeManager;
public:
    enum TYPE {
        UNUSED                  = 0x0000,      // unused frame
        TAIL                    = 0x0001,      // tail frame

        CATE_SYLLABLE           = 0x0100,
        SYLLABLE                = 0x0101,      // pinyin
        SYLLABLE_SEP            = 0x0102,      // pinyin
        INCOMPLETE_SYLLABLE     = 0x0104,      // incomplete syllable string

        CATE_OTHER              = 0x0200,
        ASCII                   = 0x0201,      // english string
        PUNC                    = 0x0202,      // punctuation
        SYMBOL                  = 0x0204,      // other symbol
        DIGITAL                 = 0x0208,      // not implemeted here
    }; // TYPE

    enum BESTWORD_TYPE {
        NO_BESTWORD             = 1 << 0,
        BESTWORD                = 1 << 1,
        USER_SELECTED           = 1 << 2,
        IGNORED                 = 1 << 3,
    }; // BESTWORD_TYPE

    unsigned m_type;
    unsigned m_bwType;
    wstring m_wstr;

    CLatticeFrame () : m_type(UNUSED), m_bwType(NO_BESTWORD) {}

    bool isUnusedFrame() const
    { return m_type == 0; }

    bool isSyllableFrame() const
    { return(m_type & CATE_SYLLABLE); }

    bool isSyllableSepFrame() const
    { return((m_type & SYLLABLE_SEP) > CATE_SYLLABLE); }

    bool isTailFrame() const
    { return(m_type == TAIL); }

	bool isNotBestWord() const
	{ return (m_bwType == CLatticeFrame::NO_BESTWORD); }

    void clear(){
        m_type = UNUSED;
        m_bwType = NO_BESTWORD;
        m_lexiconStates.clear();
        m_latticeStates.clear();
        m_wstr.clear();
        m_bestWords.clear();
    }

    void print(std::string prefix);

public:
    std::map<int, CCandidate>   m_bestWords;
    CCandidate m_selWord;
    CLexiconStates m_lexiconStates;
    CLatticeStates m_latticeStates;
}; // CLatticeFrame

typedef std::vector<CLatticeFrame>  CLattice;

#endif
