#include "syllable_seg.h"

void
TSyllableSegment::forward(unsigned i, unsigned j)
{
    std::vector<unsigned>::const_iterator it = this.m_syllables.begin();
    std::vector<unsigned>::const_iterator ite = this.m_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(i, j, *it, this);
}

static void
_forwardSingleSyllable(unsigned i,
                                    unsigned j,
                                    TSyllable syllable,
                                    const IPySegmentor::TSegment& seg,
                                    bool fuzzy)
{
    const CTrie::TNode * pn = NULL;

    CLatticeFrame &fr = this.getLatticeFrame(j);
    fr.m_type = CLatticeFrame::SYLLABLE;

    CLexiconStates::iterator it = this.getLatticeFrame(i).m_lexiconStates.begin();
    CLexiconStates::iterator ite = this.getLatticeFrame(i).m_lexiconStates.end();
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


