#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <algorithm>
#include "lexicon_states.h"


int
append(ILexiconState *lxst)
{
    m_lexiconStates.push_back(*lxst);
}

ILexiconState *
get(int i)
{
    if (i >= m_lexiconStates.size())
        return NULL
    return &m_lexiconStates[i];
}

//lastIdx = m_lattice.capacity() - 1
void
CPYLexiconStates::build(IPySegmentor::TSegmentVec &segments,
                           unsigned rebuildFrom, CLattices *lattices)
{
    IPySegmentor::TSegmentVec::const_iterator it = segments.begin();
    IPySegmentor::TSegmentVec::const_iterator ite = segments.end();

    unsigned i, j = 0;
    for (; it != ite; ++it) {
        i = it->m_start;
        j = i + it->m_len;

        if (i < rebuildFrom - 1)
            continue;

        if (j >= lattices->capacity())
            break;

        if (it->m_type == IPySegmentor::SYLLABLE)
            _forwardSyllables(i, j, *it, lattices);
        else if (it->m_type == IPySegmentor::SYLLABLE_SEP)
            _forwardSyllableSep(i, j);
        else
            _forwardString(i, j, it->m_syllables);
        m_bOmitPunct = false;
    }

    _forwardTail(j, j + 1);
    m_tailIdx = j + 1;

}

void
CIMIContext::_forwardSyllables(unsigned i,
                               unsigned j,
                               const IPySegmentor::TSegment& seg,
                               CLattices *lattices)
{
    std::vector<unsigned>::const_iterator it = seg.m_syllables.begin();
    std::vector<unsigned>::const_iterator ite = seg.m_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(i, j, *it, seg, lattices);

    it = seg.m_fuzzy_syllables.begin();
    ite = seg.m_fuzzy_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(i, j, *it, seg, lattices, true);
}


void
CIMIContext::_forwardString(unsigned i,
                            unsigned j,
                            const std::vector<unsigned>& strbuf)
{
    if (strbuf.size() == 1) {
        unsigned ch = strbuf[0];
        if (ispunct(ch)) {
            _forwardPunctChar(i, j, ch);
        } else {
            _forwardOrdinaryChar(i, j, ch);
        }
    } else{
        CLatticeFrame &fr = m_lattice[j];
        fr.m_wstr.assign(strbuf.begin(), strbuf.end());
        fr.m_lexiconStates.push_back(TLexiconState(i, 0));
    }
}

void
CIMIContext::_forwardSingleSyllable(unsigned i,
                                    unsigned j,
                                    TSyllable syllable,
                                    const IPySegmentor::TSegment& seg,
                                    bool fuzzy)
{
    const CPinyinTrie::TNode * pn = NULL;

    CLatticeFrame &fr = m_lattice[j];
    fr.m_type = CLatticeFrame::SYLLABLE;

    CLexiconStates::iterator it = m_lattice[i].m_lexiconStates.begin();
    CLexiconStates::iterator ite = m_lattice[i].m_lexiconStates.end();
    for (; it != ite; ++it) {
        TLexiconState &lxst = *it;
        bool added_from_sysdict = false;

        if (lxst.m_pPYNode) {
            // try to match a word from lattice i to lattice j
            // and if match, we'll count it as a new lexicon on lattice j
            pn = m_pPinyinTrie->transfer(lxst.m_pPYNode, syllable);
            if (pn) {
                added_from_sysdict = true;
                TLexiconState new_lxst = TLexiconState(lxst.m_start,
                                                       pn,
                                                       lxst.m_syls,
                                                       lxst.m_seg_path,
                                                       fuzzy);
                new_lxst.m_syls.push_back(syllable);
                new_lxst.m_num_of_inner_fuzzies = lxst.m_num_of_inner_fuzzies +
                                                  (seg.m_inner_fuzzy ? 1 : 0);
                new_lxst.m_seg_path.push_back(seg.m_start + seg.m_len);
                fr.m_lexiconStates.push_back(new_lxst);
            }
        }

        if (m_pUserDict && lxst.m_syls.size() < MAX_USRDEF_WORD_LEN) {
            // try to match a word from user dict
            CSyllables syls = lxst.m_syls;
            syls.push_back(syllable);
            std::vector<CPinyinTrie::TWordIdInfo> words;
            m_pUserDict->getWords(syls, words);
            if (!words.empty() || !added_from_sysdict) {
                // even if the words is empty we'll add a fake lexicon
                // here. This helps _saveUserDict detect new words.
                TLexiconState new_lxst = TLexiconState(lxst.m_start,
                                                       words,
                                                       lxst.m_syls,
                                                       lxst.m_seg_path,
                                                       fuzzy);
                new_lxst.m_syls.push_back(syllable);
                new_lxst.m_num_of_inner_fuzzies = lxst.m_num_of_inner_fuzzies +
                                                  (seg.m_inner_fuzzy ? 1 : 0);
                new_lxst.m_seg_path.push_back(seg.m_start + seg.m_len);
                fr.m_lexiconStates.push_back(new_lxst);
            }
        }
    }

    // last, create a lexicon for single character with only one syllable
    pn = m_pPinyinTrie->transfer(syllable);
    if (pn) {
        CSyllables syls;
        syls.push_back(syllable);
        std::vector<unsigned> seg_path;
        seg_path.push_back(seg.m_start);
        seg_path.push_back(seg.m_start + seg.m_len);
        TLexiconState new_lxst = TLexiconState(i, pn, syls, seg_path, fuzzy);
        new_lxst.m_num_of_inner_fuzzies = seg.m_inner_fuzzy ? 1 : 0;
        fr.m_lexiconStates.push_back(new_lxst);
    }
}

void
CIMIContext::_forwardSyllableSep(unsigned i, unsigned j)
{
    CLatticeFrame &fr = m_lattice[j];
    fr.m_type = CLatticeFrame::SYLLABLE | CLatticeFrame::SYLLABLE_SEP;
    fr.m_lexiconStates = m_lattice[i].m_lexiconStates;

    CLexiconStates::iterator it = fr.m_lexiconStates.begin();
    CLexiconStates::iterator ite = fr.m_lexiconStates.end();
    for (; it != ite; ++it) {
        it->m_seg_path.back() = j;
    }
}

void
CIMIContext::_forwardPunctChar(unsigned i, unsigned j, unsigned ch)
{
    CLatticeFrame &fr = m_lattice[j];

    wstring wstr;
    unsigned wid = 0;

    if (m_pGetFullPunctOp) {
        if (m_bFullPunctForwarding && !m_bOmitPunct) {
            wstr = (*m_pGetFullPunctOp)(ch);
            wid = m_pPinyinTrie->getSymbolId(wstr);
        }
    }

    fr.m_type = CLatticeFrame::PUNC;

    if (!wstr.empty())
        fr.m_wstr = wstr;
    else
        fr.m_wstr.push_back(ch);

    fr.m_lexiconStates.push_back(TLexiconState(i, wid));
}

void
CIMIContext::_forwardOrdinaryChar(unsigned i, unsigned j, unsigned ch)
{
    CLatticeFrame &fr = m_lattice[j];

    wstring wstr;
    unsigned wid = 0;

    if (m_pGetFullSymbolOp) {
        wstr = (*m_pGetFullSymbolOp)(ch);
        wid = m_pPinyinTrie->getSymbolId(wstr);

        if (!m_bFullSymbolForwarding)
            wstr.clear();
    }

    fr.m_type = wid ? CLatticeFrame::SYMBOL : CLatticeFrame::ASCII;

    if (!wstr.empty())
        fr.m_wstr = wstr;
    else
        fr.m_wstr.push_back(ch);

    fr.m_lexiconStates.push_back(TLexiconState(i, wid));
}

void
CIMIContext::_forwardTail(unsigned i, unsigned j)
{
    CLatticeFrame &fr = m_lattice[j];
    fr.m_type = CLatticeFrame::TAIL;

    fr.m_lexiconStates.push_back(TLexiconState(i, ENDING_WORD_ID));
}


// -*- indent-tabs-mode: nil -*- vim:et:ts=4
