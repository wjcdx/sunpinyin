#include "lattice.h"
#include "seperator_seg.h"
#include "lattice_manager.h"

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

