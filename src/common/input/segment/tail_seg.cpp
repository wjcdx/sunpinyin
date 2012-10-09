#include "tail_seg.h"
#include "lattice_manager.h"

void
TTailSegment::forward(unsigned i, unsigned j)
{
    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);
    fr.m_type = CLatticeFrame::TAIL;

    fr.m_lexiconStates.push_back(TLexiconState(i, ENDING_WORD_ID));
}
