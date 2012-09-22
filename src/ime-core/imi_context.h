
#ifndef SUNPY_IMI_CONTEXT_H
#define SUNPY_IMI_CONTEXT_H

#include "portability.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(DEBUG) && defined (HAVE_ASSET_H)
#include <assert.h>
#endif

#include <climits>
#include <map>
#include <vector>

#include "pinyin/trie/pinyin_seg.h"
#include "imi_data.h"
#include "ic_history.h"
#include "userdict.h"
#include "lattice.h"
#include "lattice_states.h"
#include "imi_funcobjs.h"
#include "candidate.h"

/**
 * TSentenceScore is only used for whole sentence score,
 * the score from language model still using double.
 */
typedef TLongExpFloat TSentenceScore;

class CLatticeFrame;
class CCandidate;
class CIMIContext;

typedef std::vector<unsigned> TPath;

class CIMIContext
{
public:
    CIMIContext ();
    ~CIMIContext () { clear(); }

    void clear();

    void setCoreData(CIMIData *pCoreData);
    void setUserDict(CUserDict *pUserDict) { m_pUserDict = pUserDict; }

    void setHistoryMemory(CICHistory *phm) { m_pHistory = phm; }
    CICHistory * getHistoryMemory() { return m_pHistory; }

    void setFullSymbolForwarding(bool value = true) {
        this.m_pLatticeMgr->setFullSymbolForwarding(value);
    }
    bool getFullSymbolForwarding() {
        return this.m_pLatticeMgr->getFullSymbolForwarding();
    }
    
    void setGetFullSymbolOp(CGetFullSymbolOp *op) {
        this.m_pLatticeMgr->setGetFullSymbolOp(op);
    }
    CGetFullSymbolOp& fullSymbolOp() const {
        return this.m_pLatticeMgr->fullSymbolOp();
    }

    void setFullPunctForwarding(bool value = true) {
        this.m_pLatticeMgr->setFullPunctForwarding(value);
    }
    bool getFullPunctForwarding() {
        return this.m_pLatticeMgr->getFullPunctForwarding();
    }
    void setGetFullPunctOp(CGetFullPunctOp *op) {
        this.m_pLatticeMgr->setGetFullPunctOp(op);
    }
    CGetFullPunctOp& fullPuncOp() const {
        return this.m_pLatticeMgr->fullPuncOp();
    }

    void setCharsetLevel(unsigned l) { m_csLevel = l; }
    unsigned getCharsetLevel() { return m_csLevel; }

    void setDynamicCandidateOrder(bool value = true) {
        m_bDynaCandiOrder = value;
    }
    bool getDynaCandiOrder() { return m_bDynaCandiOrder; }

    CLattice& getLattice() { return m_lattice; }
    bool buildLattice(IPySegmentor *segmentor, bool doSearch = true);
    
    bool isEmpty() {
        return this.m_pLatticeMgr->getTailIdx() <= 1;
    }

    unsigned getLastFrIdx() {
        return this.m_pLatticeMgr->getTailIdx() - 1;
    }

    // omit next punctuation if the very next symbol is an punctuation
    void omitNextPunct() { this.m_pLatticeMgr->setOmitPunct(true); }

    bool searchFrom(unsigned from = 1);

    size_t getMaxBest() const {
        return this.m_pLatticeMgr->getLatticeFrame(0)
                   .m_latticeStates.getMaxBest();
    }

    void setMaxBest(size_t maxBest) {
        for (int i = 0; i < MAX_LATTICE_LENGTH; i++) {
            this.m_pLatticeMgr->getLatticeFrame(i)
                .m_latticeStates.setMaxBest(maxBest);
        }
    }

    size_t getMaxTailCandidateNum() const { return m_maxTailCandidateNum; }
    void setMaxTailCandidateNum(size_t maxTailCandidateNum) {
        m_maxTailCandidateNum = maxTailCandidateNum;
    }

    size_t getNBest() { return m_nBest; }
    std::vector<TPath>& getPath(int rank) { return m_path; }
    std::vector<TPath>& getSegPath(int rank) { return m_segPath; }

    TPath& getBestPath() { return m_path[0]; }
    TPath& getBestSegPath() {
        if (m_segPath.empty()) {
            static TPath emptyPath;
            return emptyPath;
        }
        // CIMIContext would fail to backTrace the bestPathes when there are
        // no latticeStates on frame e.g., 'yiden' in Quanpin mode, in this
        // case, return the original segs
        if (m_segPath[0].empty() && m_pPySegmentor) {
            // only require the primary segments without the auxiliary ones
            IPySegmentor::TSegmentVec& segments =
                m_pPySegmentor->getSegments(false);
            IPySegmentor::TSegmentVec::const_iterator it = segments.begin();
            IPySegmentor::TSegmentVec::const_iterator ite = segments.end();
            m_segPath[0].push_back(0);
            for (; it != ite; ++it)
                m_segPath[0].push_back(it->m_start + it->m_len);
        }
        return m_segPath[0];
    }

    std::vector<CCandidates> getBestSentenceTails(int rank, unsigned start,
                                                  unsigned end = UINT_MAX);

    unsigned getBestSentence(CCandidates& result, int rank,
                             unsigned start = 0, unsigned end = UINT_MAX);
    unsigned getBestSentence(wstring& result, int rank,
                             unsigned start = 0, unsigned end = UINT_MAX);
    unsigned getBestSentence(std::vector<unsigned>& result, int rank,
                             unsigned start = 0, unsigned end = UINT_MAX);

    unsigned getSelectedSentence(wstring& result,
                                 unsigned start = 0, unsigned end = UINT_MAX);
    unsigned getSelectedSentence(std::vector<unsigned>& result,
                                 unsigned start = 0, unsigned end = UINT_MAX);

    void getCandidates(unsigned frIdx, CCandidates& result);
    unsigned cancelSelection(unsigned frIdx, bool doSearch = true);
    void makeSelection(CCandidate &candi, bool doSearch = true);
    void deleteCandidate(CCandidate &candi);
    void deleteCandidateByWID(unsigned wid);
    void selectSentence(int idx);

    void memorize();
    void removeFromHistoryCache(std::vector<unsigned>& wids);

    CUserDict* getUserDict() { return m_pUserDict; }

protected:
    void _clearFrom(unsigned from);

    bool _buildLattice(IPySegmentor::TSegmentVec &segments,
                       unsigned rebuildFrom = 1, bool doSearch = true);
    
    void _clearPaths();

    const TWCHAR *_getWstr(unsigned wid);

    void _saveUserDict();
    void _saveHistoryCache();

protected:
    size_t m_nBest;
    size_t m_maxTailCandidateNum;

    std::vector<TPath> m_path;
    std::vector<TPath> m_segPath;

    unsigned m_csLevel;

    IPySegmentor *m_pPySegmentor;

    bool m_bDynaCandiOrder;

    unsigned m_candiStarts;
    unsigned m_candiEnds;
    CLatticeManager* m_pLatticeMgr;
}; // CIMIContext

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
