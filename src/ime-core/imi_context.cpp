#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <algorithm>
#include "imi_defines.h"
#include "imi_context.h"
#include "TCandiRank.h"
#include "TCandiPair.h"
#include "TrieThreadModel.h"
#include "CInputTrieSource.h"

CIMIContext::CIMIContext()
    : m_nBest(0), m_maxBest(1), m_maxTailCandidateNum(0),
      m_csLevel(0),
      m_bDynaCandiOrder(true), m_candiStarts(0), m_candiEnds(0)
{
    m_pLatticeMgr = new CLatticeManager();
    setMaxBest(m_maxBest);
}

void
CIMIContext::setCoreData(CIMIData *pCoreData)
{
    m_pLatticeMgr->setLangModel(pCoreData->getSlm());
    CInputTrieSource::m_pTrie = pCoreData->getTrie();
}

void
CIMIContext::clear()
{
    m_pLatticeMgr->clear();
    _clearPaths();
    m_candiStarts = m_candiEnds = 0;
}

bool
CIMIContext::buildLattice(ISegmentor *segmentor, bool doSearch)
{
    m_pSegmentor = segmentor;
    
    return _buildLattice(segmentor->getSegments(),
            segmentor->updatedFrom() + 1, doSearch);
}

bool
CIMIContext::_buildLattice(TSegmentVec &segments,
                           unsigned rebuildFrom,
                           bool doSearch)
{
    _clearFrom(rebuildFrom);
    m_pLatticeMgr->buildLexiconStates(segments, rebuildFrom);
    return doSearch && searchFrom(rebuildFrom);
}

bool
CIMIContext::searchFrom(unsigned idx)
{
    bool affectCandidates = (idx <= m_candiEnds);

    static GlobalLatticeInfo info(m_csLevel, m_candiStarts, m_candiEnds);
    bool affect = m_pLatticeMgr->buildLatticeStates(idx, info);
    backTracePaths();

    return affect || affectCandidates;
}

bool
CIMIContext::backTracePaths()
{
    _clearPaths();
    m_nBest = 0;

    std::vector<TLatticeState> tail_states =
        m_pLatticeMgr->getLatticeFrame(
            m_pLatticeMgr->getTailIdx())
            .m_latticeStates.getFilteredResult();

#ifdef DEBUG
    for (int i = 0; i < tail_states.size(); i++) {
        std::string score;
        tail_states[i].m_score.toString(score);
        printf("score[%d]: %s\n", i, score.c_str());
    }
#endif

    for (size_t i = 0; i < m_maxBest; i++) {
        TPath path, segpath;
        if (m_pLatticeMgr->backTracePaths(tail_states, m_nBest, path, segpath)) {
            m_path.push_back(path);
            m_segPath.push_back(segpath);
            m_nBest++;
        }
    }

    if (m_pSegmentor && m_nBest > 0 && !m_segPath[0].empty())
        m_pSegmentor->notify_best_segpath(m_segPath[0]);
    return true;
}

void
CIMIContext::_clearPaths()
{
    m_path.clear();
    m_segPath.clear();
}

std::vector<CCandidates>
CIMIContext::getBestSentenceTails(int rank, unsigned start, unsigned end)
{
    std::vector<CCandidates> result;
    if (rank < 0) {
        return result;
    }

    CCandidates sentence;
    unsigned word_num = getBestSentence(sentence, rank, start, end);
    unsigned tail_word_num = word_num;

    while (tail_word_num > 1) {
        unsigned dec = tail_word_num / (m_maxTailCandidateNum + 1) + 1;
        tail_word_num -= std::min(dec, tail_word_num);
        if (tail_word_num <= 1) {
            break;
        }
        CCandidates tail(sentence.begin(), sentence.begin() + tail_word_num);
        result.push_back(tail);
    }
    return result;
}

unsigned
CIMIContext::getBestSentence(CCandidates& result, int rank,
                             unsigned start, unsigned end)
{
    // -1 means selected sentence
    if (rank < -1 || rank >= (int) m_nBest)
        return 0;

    result.clear();

    if (end == UINT_MAX)
        end = getLastFrIdx();

    while (end > start && m_pLatticeMgr->getLatticeFrame(end)
            .isNotBestWord())
        end--;

    unsigned i = end, nWordConverted = 0;
    while (i > start) {
        CLatticeFrame& fr = m_pLatticeMgr->getLatticeFrame(i);
        if (rank < 0) {
            result.insert(result.begin(), fr.m_selWord);
            i = fr.m_selWord.m_start;
        } else {
            result.insert(result.begin(), fr.m_bestWords[rank]);
            i = fr.m_bestWords[rank].m_start;
        }
        nWordConverted++;
    }
    return nWordConverted;
}

unsigned
CIMIContext::getBestSentence(wstring& result, int rank,
                             unsigned start, unsigned end)
{
    CCandidates sentence;
    unsigned nWordConverted = getBestSentence(sentence, rank, start, end);
    result.clear();
    for (size_t i = 0; i < sentence.size(); i++) {
        result += sentence[i].m_cwstr;
    }
    return nWordConverted;
}

unsigned
CIMIContext::getBestSentence(std::vector<unsigned>& result, int rank,
                             unsigned start, unsigned end)
{
    CCandidates sentence;
    unsigned nWordConverted = getBestSentence(sentence, rank, start, end);
    result.clear();
    for (size_t i = 0; i < sentence.size(); i++) {
        result.push_back(sentence[i].m_wordId);
    }
    return nWordConverted;
}


unsigned
CIMIContext::getSelectedSentence(wstring& result,
                                 unsigned start, unsigned end)
{
    return getBestSentence(result, -1, start, end);
}


unsigned
CIMIContext::getSelectedSentence(std::vector<unsigned>& result,
                                 unsigned start, unsigned end)
{
    return getBestSentence(result, -1, start, end);
}

void
CIMIContext::getCandidates(unsigned frIdx, CCandidates& result)
{
    TCandiPair cp;
    static std::map<wstring, TCandiPair> candidates_map;
    std::map<wstring, TCandiPair>::iterator candidates_it;

    candidates_map.clear();
    result.clear();

    std::vector<unsigned> st;
    getSelectedSentence(st, frIdx);

    cp.m_candi.m_start = m_candiStarts = frIdx++;

    for (; frIdx <= getLastFrIdx(); ++frIdx) {
        if (m_pLatticeMgr->getLatticeFrame(frIdx + 1).isSyllableSepFrame())
            continue;

        CLatticeFrame &fr = m_pLatticeMgr->getLatticeFrame(frIdx);
        if (!fr.isSyllableFrame())
            continue;

        cp.m_candi.m_end = frIdx;
        if (!fr.isNotBestWord()) {
            for (size_t i = 0; i < m_nBest; i++) {
                if (fr.m_bestWords.find(i) == fr.m_bestWords.end())
                    continue;
                CCandidate candi = fr.m_bestWords[i];
                if (candi.m_start != m_candiStarts)
                    continue;
                if (candi.m_pLexiconState == NULL)
                    continue;

                TLexiconState & lxst = *(candi.m_pLexiconState);
                int len = lxst.getLength();
                if (len == 0) len = 1;

                cp.m_candi = candi;
                cp.m_Rank =
                    TCandiRank(fr.m_bwType & CLatticeFrame::USER_SELECTED,
                               fr.m_bwType & CLatticeFrame::BESTWORD,
                               len, false, 0);
                candidates_map[candi.m_cwstr] = cp;
            }
        }

        bool found = false;
        CLexiconStates::iterator it = fr.m_lexiconStates.begin();
        CLexiconStates::iterator ite = fr.m_lexiconStates.end();
        for (; it != ite; ++it) {
            TLexiconState & lxst = *it;

            if (lxst.m_start != m_candiStarts)
                continue;

            int len = lxst.getLength();
            if (0 == len) len = 1;

            found = true;
            unsigned word_num;
            const TWordIdInfo *words = lxst.getWords(word_num);

            for (unsigned i = 0; i < word_num; ++i) {
                if (m_csLevel < words[i].m_csLevel)
                    continue;

                cp.m_candi.m_wordId = words[i].m_id;
                cp.m_candi.m_cwstr = CInputTrieSource::getWstr(cp.m_candi.m_wordId);
                cp.m_candi.m_pLexiconState = &lxst;
                if (!cp.m_candi.m_cwstr)
                    continue;

                //sorting according to the order in PinYinTire
                cp.m_Rank =
                    TCandiRank(false,
                               !st.empty() && st.front() == cp.m_candi.m_wordId,
                               len, false, i);
                candidates_it = candidates_map.find(cp.m_candi.m_cwstr);
                if (candidates_it == candidates_map.end()
                    || cp.m_Rank < candidates_it->second.m_Rank
                    || cp.m_candi.m_wordId > INI_USRDEF_WID) {
                    candidates_map[cp.m_candi.m_cwstr] = cp;
                    // print_wide(cp.m_candi.m_cwstr);
                    // printf(" ");
                }
            }
            // puts("");
        }

        if (!found) continue;  // FIXME: need better solution later

        if (m_bDynaCandiOrder) {
            CLatticeStates::iterator it = fr.m_latticeStates.begin();
            CLatticeStates::iterator ite = fr.m_latticeStates.end();
            // printf("adjusting ");
            for (; it != ite; ++it) {
                TLatticeState & ltst = *it;

                if (ltst.m_pBackTraceNode->m_frIdx != m_candiStarts)
                    continue;

                cp.m_candi.m_wordId = ltst.m_backTraceWordId;
                cp.m_candi.m_cwstr = CInputTrieSource::getWstr(cp.m_candi.m_wordId);
                cp.m_candi.m_pLexiconState = ltst.m_pLexiconState;
                if (!cp.m_candi.m_cwstr)
                    continue;

                int len = cp.m_candi.m_pLexiconState->getLength();
                if (0 == len) len = 1;
                cp.m_Rank = TCandiRank(false,
                                       !st.empty() && st.front() ==
                                       cp.m_candi.m_wordId,
                                       len, true, ltst.m_score /
                                       ltst.m_pBackTraceNode->m_score);
                candidates_it = candidates_map.find(cp.m_candi.m_cwstr);
                if (candidates_it == candidates_map.end()
                    || cp.m_Rank < candidates_it->second.m_Rank
                    || cp.m_candi.m_wordId > INI_USRDEF_WID) {
                    // print_wide(cp.m_candi.m_cwstr);
                    // std::string buf;
                    // ltst.m_score.toString(buf);
                    // printf("len:%d %s", len, buf.c_str());
                    // ltst.m_pBackTraceNode->m_score.toString(buf);
                    // printf("%s ", buf.c_str());
                    candidates_map[cp.m_candi.m_cwstr] = cp;
                }
            }
            // puts("");
        }

        m_candiEnds = frIdx;
    }

    std::vector<TCandiPairPtr> vec;

    vec.reserve(candidates_map.size());
    for (candidates_it = candidates_map.begin();
         candidates_it != candidates_map.end(); ++candidates_it) {
        vec.push_back(TCandiPairPtr(&(candidates_it->second)));
    }

    std::sort(vec.begin(), vec.end());
    for (size_t i = 0; i < vec.size(); i++) {
        // print_wide(vec[i].m_Ptr->m_candi.m_cwstr);
        // printf(" ");
        result.push_back(vec[i].m_Ptr->m_candi);
    }
    // puts("");
}

unsigned
CIMIContext::cancelSelection(unsigned frIdx, bool doSearch)
{
    unsigned ret = frIdx;

    CLatticeFrame &fr = m_pLatticeMgr->getLatticeFrame(frIdx);
    while (fr.m_bwType & CLatticeFrame::IGNORED) {
        --frIdx;
        fr = m_pLatticeMgr->getLatticeFrame(frIdx);
    }

    if (fr.m_bwType &
        (CLatticeFrame::USER_SELECTED | CLatticeFrame::BESTWORD)) {
        ret = fr.m_selWord.m_start;
        fr.m_bwType = CLatticeFrame::NO_BESTWORD;
        if (doSearch) searchFrom(frIdx);
    }

    return ret;
}

void
CIMIContext::makeSelection(CCandidate &candi, bool doSearch)
{
    CLatticeFrame &fr = m_pLatticeMgr->getLatticeFrame(candi.m_end);
    fr.m_bwType = fr.m_bwType | CLatticeFrame::USER_SELECTED;
    fr.m_selWord = candi;
    // make best sentence word consistent as well
    for (size_t i = 0; i < m_nBest; i++) {
        fr.m_bestWords[i] = candi;
    }

    if (doSearch) searchFrom(candi.m_end);
}

void
CIMIContext::selectSentence(int idx)
{
    unsigned i = getLastFrIdx();
    while (i > 0 && m_pLatticeMgr->getLatticeFrame(i).isNotBestWord())
        i--;

    while (i > 0) {
        CLatticeFrame &fr = m_pLatticeMgr->getLatticeFrame(i);
        fr.m_selWord = fr.m_bestWords[idx];
        i = fr.m_selWord.m_start;
    }
}

void
CIMIContext::memorize()
{
    _saveUserDict();
    _saveHistoryCache();
}

void
CIMIContext::_saveUserDict()
{
    if (!CInputTrieSource::m_pUserDict)
        return;

    CSyllables syls;
    bool has_user_selected = false;
    unsigned i = getLastFrIdx();
    unsigned e_pos = 0;

    while (i > 0 && m_pLatticeMgr->getLatticeFrame(i).isNotBestWord())
        i--;

    while (i > 0) {
        CLatticeFrame &fr = m_pLatticeMgr->getLatticeFrame(i);
        if (!fr.isSyllableFrame()) {
            i = fr.m_selWord.m_start;
            break;
        }

        TLexiconState* state = fr.m_selWord.m_pLexiconState;
        if (!state) {
            i = fr.m_selWord.m_start;
            continue;
        }

        if (syls.size() + state->m_syls.size() > MAX_USRDEF_WORD_LEN) {
            i = fr.m_selWord.m_start;
            break;
        }

    if (!e_pos) e_pos = i;

        has_user_selected |= (fr.m_bwType & CLatticeFrame::USER_SELECTED);
        std::copy(state->m_syls.begin(), state->m_syls.end(), inserter(syls, syls.begin()));
        i = fr.m_selWord.m_start;
    }

    if (has_user_selected && syls.size() > 1) {
        wstring phrase;
        getSelectedSentence (phrase, 0, e_pos);
        CInputTrieSource::m_pUserDict->addWord (syls, phrase);
    }
}

void
CIMIContext::_saveHistoryCache()
{
    if (!m_pHistory)
        return;

    std::vector<unsigned> result;
    unsigned i = getLastFrIdx();
    while (i > 0 && m_pLatticeMgr->getLatticeFrame(i).isNotBestWord())
        i--;

    while (i > 0) {
        CLatticeFrame &fr = m_pLatticeMgr->getLatticeFrame(i);
        if (fr.isSyllableFrame()) {
            result.insert(result.begin(), fr.m_selWord.m_wordId);
        } else {
            result.insert(result.begin(), 0);
        }
        i = fr.m_selWord.m_start;
    }

    if (!result.empty()) {
        m_pHistory->memorize(&(result[0]), &(result[0]) + result.size());
        m_pHistory->saveToFile();
    }
}

void
CIMIContext::deleteCandidate(CCandidate &candi)
{
    unsigned wid = candi.m_wordId;
    deleteCandidateByWID(wid);
}

void
CIMIContext::deleteCandidateByWID(unsigned wid)
{
    if (wid > INI_USRDEF_WID) {
        m_pHistory->forget(wid);
        CInputTrieSource::m_pUserDict->removeWord(wid);
        _buildLattice(m_pSegmentor->getSegments());
    }
}

void
CIMIContext::removeFromHistoryCache(std::vector<unsigned>& wids)
{
    if (!m_pHistory)
        return;

    m_pHistory->forget(&(wids[0]), &(wids[0]) + wids.size());
    buildLattice(m_pSegmentor);
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
