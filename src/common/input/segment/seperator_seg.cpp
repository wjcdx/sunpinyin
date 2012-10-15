#include "ime-core/lattice/lattice.h"
#include "ime-core/lattice/lattice_manager.h"
#include "seperator_seg.h"

void
TSeperatorSegment::forward(unsigned i, unsigned j)
{
    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j); 
    fr.m_type = CLatticeFrame::SYLLABLE | CLatticeFrame::SYLLABLE_SEP; 
    fr.m_lexiconStates = CLatticeManager::getLatticeFrame(i).m_lexiconStates;

    CLexiconStates::iterator it = fr.m_lexiconStates.begin();
    CLexiconStates::iterator ite = fr.m_lexiconStates.end();
    for (; it != ite; ++it) {
        it->m_seg_path.back() = j;
    }
}

