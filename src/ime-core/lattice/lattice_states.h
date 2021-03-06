#ifndef SUNPY_LATTICE_STATES_H
#define SUNPY_LATTICE_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "ime-core/imi_data.h"
#include "pinyin/input/pinyin_seg.h"
#include "common/lexicon/trie.h"
#include "lexicon_states.h"

typedef TLongExpFloat TSentenceScore;

/**
 * CSlmState represent the history. In real implementation, it's a
 * node pointer to a state in the language model. But to save the
 * language model size, the state node in language model do not
 * thread the back-off pointer. Now, we just use the Word Id for
 * the node in the language model. Later we should abstract the
 * StateNode from language model implemetation to replace this
 * definition.
 */
typedef CThreadSlm::TState CSlmState;

/**
 * A WordKey could represent a word. Define this use the unsigned int
 * directly. Because in the future, we may adopt word class, such as
 * Digital Word Class.
 */
typedef unsigned CWordId;

/**
 * The basic static unit used in the lattice searching
 */
struct TLatticeState {
    TSentenceScore m_score;
    unsigned m_frIdx;
    TLexiconState* m_pLexiconState;
    TLatticeState* m_pBackTraceNode;
    CSlmState m_slmState;
    CWordId m_backTraceWordId;

    TLatticeState(double score = -1.0,
                  unsigned frIdx = 0,
                  TLexiconState* lxstPtr = NULL,
                  TLatticeState* btNodePtr = NULL,
                  CSlmState sk = CSlmState(),
                  CWordId wk = CWordId())
    : m_score(score), m_frIdx(frIdx), m_pLexiconState(lxstPtr),
        m_pBackTraceNode(btNodePtr), m_slmState(sk), m_backTraceWordId(wk) {}

    /** for debug printing... */
    void print(std::string prefix) const;

    bool operator<(const TLatticeState& rhs) const {
        return m_score < rhs.m_score;
    }

    bool operator==(const TLatticeState& rhs) const {
        return m_score == rhs.m_score;
    }

    bool operator>(const TLatticeState& rhs) const {
        return !((*this) < rhs || (*this) == rhs);
    }
};

typedef std::vector<TLatticeState>  CLatticeStateVec;

/**
 * A container that keeps the top K elements.
 */
class CTopLatticeStates {
    std::vector<TLatticeState> m_heap;
    size_t m_threshold;
public:
    CTopLatticeStates(size_t threshold) : m_threshold(threshold) {}

    bool push(const TLatticeState& state);
    void pop();

    const TLatticeState& top() const { return m_heap[0]; }

    size_t size() const { return m_heap.size(); }

    typedef std::vector<TLatticeState>::iterator iterator;

    iterator begin() { return m_heap.begin(); }
    iterator end() { return m_heap.end(); }
};

/**
 * All lattice node on a lattice frame. This class provide beam pruning
 * while push_back, which means at most the best MAX states are reserved,
 * ie, weak state will may be discard while new better state are inserted,
 * and the number MAX is arrived.
 */
class CLatticeStates {
private:
    static const unsigned beam_width;
    static const TSentenceScore filter_ratio_l1;
    static const TSentenceScore filter_ratio_l2;
    static const TSentenceScore filter_threshold_exp;

public:
    CLatticeStates() : m_size(0), m_maxBest(2) {}

    void setMaxBest(size_t maxBest) { m_maxBest = maxBest; }
    size_t getMaxBest() { return m_maxBest; }

    void clear();
    void add(const TLatticeState& state);

    std::vector<TLatticeState> getSortedResult();
    std::vector<TLatticeState> getFilteredResult();

    typedef std::map<CSlmState, CTopLatticeStates> state_map;
    class iterator {
        friend class CLatticeStates;
        state_map::iterator m_mainIt;
        state_map::iterator m_mainEnd;
        CTopLatticeStates::iterator m_childIt;
public:
        iterator(state_map::iterator mit, state_map::iterator mend,
                 CTopLatticeStates::iterator cit)
            : m_mainIt(mit), m_mainEnd(mend), m_childIt(cit) {}

        iterator() {}

        void operator++();
        bool operator!=(const CLatticeStates::iterator& rhs);
        TLatticeState& operator*();
        TLatticeState* operator->();
    };

    iterator begin();
    iterator end();

    size_t size() {
        return m_size;
    }
private:
    void _pushScoreHeap(TSentenceScore score, CSlmState slmState);
    void _popScoreHeap();
    void _refreshHeapIdx(int heapIdx);
    void _adjustUp(int node);
    void _adjustDown(int node);

private:
    state_map m_stateMap;
    size_t m_size;
    size_t m_maxBest;

    std::map<CSlmState, int>                           m_heapIdx;
    std::vector<std::pair<TSentenceScore, CSlmState> > m_scoreHeap;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
