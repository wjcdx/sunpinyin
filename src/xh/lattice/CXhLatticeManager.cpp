#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <algorithm>
#include "lexicon_states.h"

void
CXhLatticeManager::buildLexiconStates(ISegmentor::TSegmentVec &segments,
                           unsigned rebuildFrom)
{
    ISegmentor::TSegmentVec::const_iterator it = segments.begin();
    ISegmentor::TSegmentVec::const_iterator ite = segments.end();

    unsigned i, j = 0;
    for (; it != ite; ++it) {
        i = it->m_start;
        j = i + it->m_len;

        if (i < rebuildFrom - 1)
            continue;

        if (j >= this.getLatticesCapacity())
            break;

		(*it).forward(i, j);
		CFullCharManager::m_bOmitPunct = false;
    }

    TTailSegment.forward(j, j + 1);
    m_tailIdx = j + 1;
}

