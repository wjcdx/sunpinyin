#include "tail_seg.h"

void
TTailSegment::forward(unsigned i, unsigned j)
{
    CLatticeFrame &fr = this.getLatticeFrame(j);
    fr.m_type = CLatticeFrame::TAIL;

    fr.m_lexiconStates.push_back(TLexiconState(i, ENDING_WORD_ID));
}
