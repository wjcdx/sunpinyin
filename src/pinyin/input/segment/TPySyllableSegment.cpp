#include "syllable_seg.h"
#include "lattice.h"
#include "TPyLexiconState.h"
#include "TrieThreadModel.h"
#include "lattice_manager.h"
#include "CInputTrieSource.h"

using namespace TrieThreadModel;

void
TPySyllableSegment::forward(unsigned i, unsigned j)
{
    std::vector<unsigned>::const_iterator it = m_syllables.begin();
    std::vector<unsigned>::const_iterator ite = m_syllables.end();

    for (; it != ite; ++it)
        _forwardSingleSyllable(i, j, *it, true);
}

void
TPySyllableSegment::_forwardSingleSyllable(unsigned i,
						unsigned j,
						TSyllable syllable,
						bool fuzzy)
{
    const TThreadNode * pn = NULL;

    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);
    fr.m_type = CLatticeFrame::SYLLABLE;

    CLexiconStates::iterator it = CLatticeManager::getLatticeFrame(i).m_lexiconStates.begin();
    CLexiconStates::iterator ite = CLatticeManager::getLatticeFrame(i).m_lexiconStates.end();
    for (; it != ite; ++it) {
        TPyLexiconState &lxst = *(TPyLexiconState *)(&(*it));
        bool added_from_sysdict = false;

        if (lxst.m_pNode) {
            // try to match a word from lattice i to lattice j
            // and if match, we'll count it as a new lexicon on lattice j
            pn = CInputTrieSource::m_pTrie->transfer(lxst.m_pNode, syllable);
            if (pn) {
                added_from_sysdict = true;
                TPyLexiconState new_lxst = TPyLexiconState(lxst.m_start,
                                                       pn,
                                                       lxst.m_syls,
                                                       lxst.m_seg_path,
                                                       fuzzy);
                new_lxst.m_syls.push_back(syllable);
                new_lxst.m_num_of_inner_fuzzies = lxst.m_num_of_inner_fuzzies +
                                                  (this->m_inner_fuzzy ? 1 : 0);
                new_lxst.m_seg_path.push_back(this->m_start + this->m_len);
                fr.m_lexiconStates.push_back(new_lxst);
            }
        }

        if (CInputTrieSource::m_pUserDict && lxst.m_syls.size() < MAX_USRDEF_WORD_LEN) {
            // try to match a word from user dict
            CSyllables syls = lxst.m_syls;
            syls.push_back(syllable);
            std::vector<TWordIdInfo> words;
			CInputTrieSource::m_pUserDict->getWords(syls, words);
            if (!words.empty() || !added_from_sysdict) {
                // even if the words is empty we'll add a fake lexicon
                // here. This helps _saveUserDict detect new words.
                TPyLexiconState new_lxst = TPyLexiconState(lxst.m_start,
                                                       words,
                                                       lxst.m_syls,
                                                       lxst.m_seg_path,
                                                       fuzzy);
                new_lxst.m_syls.push_back(syllable);
                new_lxst.m_num_of_inner_fuzzies = lxst.m_num_of_inner_fuzzies +
                                                  (this->m_inner_fuzzy ? 1 : 0);
                new_lxst.m_seg_path.push_back(this->m_start + this->m_len);
                fr.m_lexiconStates.push_back(new_lxst);
            }
        }
    }

    // last, create a lexicon for single character with only one syllable
    pn = CInputTrieSource::m_pTrie->transfer(syllable);
    if (pn) {
        CSyllables syls;
        syls.push_back(syllable);
        std::vector<unsigned> seg_path;
        seg_path.push_back(this->m_start);
        seg_path.push_back(this->m_start + this->m_len);
        TPyLexiconState new_lxst = TPyLexiconState(i, pn, syls, seg_path, fuzzy);
        new_lxst.m_num_of_inner_fuzzies = this->m_inner_fuzzy ? 1 : 0;
        fr.m_lexiconStates.push_back(new_lxst);
    }
}



