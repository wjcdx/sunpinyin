#include "seperator_seg.h"

void
TSeperatorSegment::forward(unsigned i, unsigned j)
{
    CLatticeFrame &fr = this.getLatticeFrame(j); 
    fr.m_type = CLatticeFrame::SYLLABLE | CLatticeFrame::SYLLABLE_SEP; 
    fr.m_lexiconStates = this.getLatticeFrame(i).m_lexiconStates;

    CLexiconStates::iterator it = fr.m_lexiconStates.begin();
    CLexiconStates::iterator ite = fr.m_lexiconStates.end();
    for (; it != ite; ++it) {
        it->m_seg_path.back() = j;     
    }
}

