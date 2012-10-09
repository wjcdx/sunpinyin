#include "trie.h"
#include "lattice.h"
#include "lattice_manager.h"

CLattice CLatticeManager::m_lattice;
CTrie *CLatticeManager::m_pTrie;

void
CLatticeManager::printLattice()
{
    std::string prefix;

    for (size_t i = 0; i <= m_tailIdx; ++i) {
        if (m_lattice[i].m_type == CLatticeFrame::UNUSED)
            continue;

        printf("Lattice Frame [%lu]:", i);
        m_lattice[i].print(prefix);
    }
}

void
CLatticeManager::clear()
{
    _clearFrom(1);
	this.setTailIdx(1);
}

void
CLatticeManager::clearFrom(unsigned idx)
{
    for (size_t i = idx; i < m_tailIdx + 1; i++)
        m_lattice[i].clear();
}

static double exp2_tbl[32] = {
    exp2(-0), exp2(-1), exp2(-2), exp2(-3), exp2(-4), exp2(-5), exp2(-6), exp2(-7),
    exp2(-8), exp2(-9), exp2(-10), exp2(-11), exp2(-12), exp2(-13), exp2(-14),
    exp2(-15), exp2(-16), exp2(-17), exp2(-18), exp2(-19), exp2(-20), exp2(-21),
    exp2(-22), exp2(-23), exp2(-24), exp2(-25), exp2(-26), exp2(-27), exp2(-28),
    exp2(-29), exp2(-30), exp2(-31)
};

bool
CLatticeManager::buildLatticeStates(int rebuildFrom, unsigned csLevel, bool affectCandidates)
{
    for (; rebuildFrom <= m_tailIdx; ++rebuildFrom) {
        CLatticeFrame &fr = m_lattice[rebuildFrom];

        if (fr.m_type == CLatticeFrame::UNUSED)
            continue;

        fr.m_latticeStates.clear();

        /* user selected word might be cut in next step */
        if (fr.m_bwType & CLatticeFrame::USER_SELECTED) {
            _transferBetween(fr.m_selWord.m_start, rebuildFrom,
                             fr.m_selWord.m_pLexiconState,
                             fr.m_selWord.m_wordId);
        }

        CLexiconStates::iterator it = fr.m_lexiconStates.begin();
        CLexiconStates::iterator ite = fr.m_lexiconStates.end();
        for (; it != ite; ++it) {
            unsigned word_num = 0;
            TLexiconState &lxst = *it;
            const CPinyinTrie::TWordIdInfo *words = lxst.getWords(word_num);

            if (!word_num)
                continue;

            if (lxst.m_start == m_candiStarts && rebuildFrom > m_candiEnds)
                affectCandidates = true;

            // only selected the word with higher unigram probablities, and
            // narrow the search deepth and lower the initial score for fuzzy
            // syllables
            double ic = lxst.getWeight();
            int maxsz = (int)(ic * MAX_LEXICON_TRIES);


            int sz = (int) word_num < maxsz ? (int) word_num : maxsz;
            int i = 0, count = 0;

            while (count < sz && i < sz && (words[i].m_bSeen || count < 2)) {
                if (csLevel >= words[i].m_csLevel) {
                    // printf("cost %d\n", words[i].m_cost);
                    _transferBetween(lxst.m_start, rebuildFrom, &lxst, words[i].m_id,
                                     ic * exp2_tbl[words[i].m_cost]);
                    ++count;
                }
                i++;
            }

            /* try extra words in history cache */
            if (m_pHistory) {
                while (i < (int) word_num) {
                    if (csLevel >= words[i].m_csLevel
                        && m_pHistory->seenBefore(words[i].m_id)) {
                        // printf("history cost %d\n", words[i].m_cost);
                        _transferBetween(lxst.m_start, rebuildFrom, &lxst,
                                         words[i].m_id,
                                         ic * exp2_tbl[words[i].m_cost]);
                    }
                    i++;
                }
            }
        }
    }
}

void
CLatticeManager::_transferBetween(unsigned start, unsigned end,
                              TLexiconState* plxst, unsigned wid,
                              double ic)
{
    CLatticeFrame &start_fr = m_lattice[start];
    CLatticeFrame &end_fr = m_lattice[end];

    TLatticeState node(-1.0, end, plxst);
    TSentenceScore efic(ic);

    if ((end_fr.m_bwType & CLatticeFrame::USER_SELECTED)
        && end_fr.m_selWord.m_wordId == wid)
        efic = TSentenceScore(30000, 1.0);

    static double s_history_distribution[] = {
        0.0, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50
    };

    double weight_h = s_history_distribution[m_historyPower];
    double weight_s = 1.0 - weight_h;

    CLatticeStates::iterator it = start_fr.m_latticeStates.begin();
    CLatticeStates::iterator ite = start_fr.m_latticeStates.end();

    for (; it != ite; ++it) {
        // for 1-length lattice states, replace ending_word_id (comma)
        // with none_word_id (recognized by CThreadSlm)
    	unsigned _wid = wid;
        if (wid == ENDING_WORD_ID && it->m_pBackTraceNode && it->m_pBackTraceNode->m_frIdx == 0)
            _wid = NONE_WORD_ID;

        node.m_pBackTraceNode = &(*it);
        node.m_backTraceWordId = wid;

        double ts = m_pModel->transfer(it->m_slmState, _wid, node.m_slmState);
        m_pModel->historify(node.m_slmState);

        // backward to psuedo root, so wid is probably a user word,
        // save the wid in idx field, so that later we could get it via
        // CThreadSlm::lastWordId, to calculate p_{cache} correctly.
        if (node.m_slmState.getLevel() == 0
            && m_pHistory && m_pHistory->seenBefore(wid))
            node.m_slmState.setIdx(wid);  // an psuedo unigram node state

        if (m_pHistory) {
            unsigned history[2] = { m_pModel->lastWordId(it->m_slmState), _wid };
            double hpr = m_pHistory->pr(history, history + 2);
            ts = weight_s * ts + weight_h * hpr;
        }

        node.m_score = it->m_score * efic * TSentenceScore(ts);
        end_fr.m_latticeStates.add(node);
    }
}

bool
CLatticeManager::backTracePaths(const std::vector<TLatticeState>& tail_states,
                             int rank, TPath& path, TPath& segmentPath)
{
    path.clear();
    segmentPath.clear();

    if (rank >= (int) tail_states.size()) {
        // rank out of bounds, only return the segment path
        return false;
    }

    const TLatticeState *bs = &(tail_states[rank]);

    while (bs->m_pBackTraceNode) {
        unsigned start = bs->m_pBackTraceNode->m_frIdx;
        unsigned end = bs->m_frIdx;
        CLatticeFrame & end_fr = m_lattice[end];

        if (!(end_fr.m_bwType & CLatticeFrame::USER_SELECTED)) {
            const TWCHAR* cwstr = NULL;
            if (end_fr.m_wstr.empty()) {
                cwstr = _getWstr(bs->m_backTraceWordId);
            } else {
                cwstr = end_fr.m_wstr.c_str();
            }

            CCandidate candi(start, end, bs->m_pLexiconState, cwstr,
                             bs->m_backTraceWordId);

            end_fr.m_bwType |= CLatticeFrame::BESTWORD;
            end_fr.m_bestWords[rank] = candi;
            if (rank == 0) {
                end_fr.m_selWord = candi; // select the first by default.
            }
        }

        if (bs->m_pBackTraceNode->m_pLexiconState) {
            std::vector<unsigned> seg_path =
                bs->m_pBackTraceNode->m_pLexiconState->m_seg_path;
            std::vector<unsigned>::reverse_iterator it = seg_path.rbegin();

            for (; it != seg_path.rend(); ++it) {
                if (segmentPath.empty() || segmentPath.back() != *it)
                    segmentPath.push_back(*it);
            }
        }

        path.push_back(end);
        bs = bs->m_pBackTraceNode;
    }

    std::reverse(path.begin(), path.end());
    std::reverse(segmentPath.begin(), segmentPath.end());

#ifdef DEBUG
    std::vector<unsigned>::iterator it;

    printf("trace lattice path[%d]: ", rank);
    for (it = path.begin(); it != path.end(); ++it)
        printf("%d ", *it);
    printf("\n");

    printf("trace segments path[%d]: ", rank);
    for (it = segmentPath.begin(); it != segmentPath.end(); ++it)
        printf("%d ", *it);
    printf("\n");
#endif

    return true;
}

