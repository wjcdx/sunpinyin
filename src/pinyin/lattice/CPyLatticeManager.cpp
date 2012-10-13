#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <algorithm>
#include "TPyLexiconState.h"
#include "CPyLatticeManager.h"
#include "CFullCharManager.h"


void
CPyLatticeManager::buildLexiconStates(TSegmentVec &segments,
                           unsigned rebuildFrom)
{
    TSegmentVec::iterator it = segments.begin();
    TSegmentVec::iterator ite = segments.end();

    unsigned i, j = 0;
    for (; it != ite; ++it) {
        i = it->m_start;
        j = i + it->m_len;

        if (i < rebuildFrom - 1)
            continue;

        if (j >= getLatticesCapacity())
            break;
        (*it).forward(i, j); 
        CFullCharManager::m_bOmitPunct = false;
    }

    TTailSegment::forward(j, j + 1);
    m_tailIdx = j + 1;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
