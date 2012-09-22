#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <algorithm>
#include "lexicon_states.h"

void
CPYLatticeManager::buildLexiconStates(IPySegmentor::TSegmentVec &segments,
                           unsigned rebuildFrom)
{
    IPySegmentor::TSegmentVec::const_iterator it = segments.begin();
    IPySegmentor::TSegmentVec::const_iterator ite = segments.end();

    unsigned i, j = 0;
    for (; it != ite; ++it) {
        i = it->m_start;
        j = i + it->m_len;

        if (i < rebuildFrom - 1)
            continue;

        if (j >= this.getLatticesCapacity())
            break;

        if (it->m_type == IPySegmentor::SYLLABLE)
            _forwardSyllables(i, j, *it);
        else if (it->m_type == IPySegmentor::SYLLABLE_SEP)
            _forwardSyllableSep(i, j);
        else
            _forwardString(i, j, it->m_syllables);
        m_bOmitPunct = false;
    }

    _forwardTail(j, j + 1);
    m_tailIdx = j + 1;
}

