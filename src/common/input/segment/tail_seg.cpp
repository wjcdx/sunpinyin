#include "ime-core/lattice/lattice_manager.h"
#include "tail_seg.h"

void
TTailSegment::forward(unsigned i, unsigned j)
{
    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);
    fr.m_type = CLatticeFrame::TAIL;

    fr.m_lexiconStates.push_back(TLexiconState(i, ENDING_WORD_ID));
}
