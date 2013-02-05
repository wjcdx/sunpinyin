#include "lattice_states.h"

void
TLatticeState::print(std::string prefix) const
{
    printf("%s", prefix.c_str());
    char valbuf[256];
    m_score.toString(valbuf);
    printf("<State(%d:%d), from word %d, score %s>\n", m_slmState.getLevel(),
           m_slmState.getIdx(), m_backTraceWordId, valbuf);
}

const unsigned CLatticeStates::beam_width = 48;
const TSentenceScore CLatticeStates::filter_ratio_l1 = TSentenceScore(0.12);
const TSentenceScore CLatticeStates::filter_ratio_l2 = TSentenceScore(0.02);
const TSentenceScore CLatticeStates::filter_threshold_exp =
    TSentenceScore(-40, -1.0);

bool
CTopLatticeStates::push(const TLatticeState& state)
{
    bool ret = true;
    state.print("m_heap A: ");
    if (size() >= m_threshold) {
        if (m_heap[0] < state) return false;
        std::pop_heap(m_heap.begin(), m_heap.end());
        m_heap.back().print("m_heap R: ");
        m_heap.pop_back();
        ret = false;
    }
    m_heap.push_back(state);
    std::push_heap(m_heap.begin(), m_heap.end());
    return ret;
}

void
CTopLatticeStates::pop()
{
    std::pop_heap(m_heap.begin(), m_heap.end());
    m_heap.back().print("m_heap R: ");
    m_heap.pop_back();
}

void
CLatticeStates::clear()
{
    m_heapIdx.clear();
    m_scoreHeap.clear();
    m_stateMap.clear();
    m_size = 0;
}

std::vector<TLatticeState>
CLatticeStates::getSortedResult()
{
    std::vector<TLatticeState> res;
    for (CLatticeStates::iterator it = begin(); it != end(); ++it) {
        res.push_back(*it);
    }
    std::sort(res.begin(), res.end());
    return res;
}

std::vector<TLatticeState>
CLatticeStates::getFilteredResult()
{
    std::vector<TLatticeState> sorted = getSortedResult();
    std::vector<TLatticeState> res;
    if (sorted.size() == 0) {
        return sorted;
    }
    res.push_back(sorted[0]);
    TSentenceScore max_score = sorted[0].m_score;
    for (size_t i = 1; i < sorted.size(); i++) {
        TSentenceScore current_score = sorted[i].m_score;
        if (filter_threshold_exp < current_score
            && current_score / max_score < filter_ratio_l1) {
            break;
        }
        if (current_score / max_score < filter_ratio_l2) {
            break;
        }
        res.push_back(sorted[i]);
    }
    return res;
}

void
CLatticeStates::add(const TLatticeState& state)
{
    CSlmState slmState = state.m_slmState;
    state_map::iterator it = m_stateMap.find(slmState);
    bool inserted = false;

    if (it == m_stateMap.end()) {
        CTopLatticeStates topstates(m_maxBest);
        inserted = topstates.push(state);
        m_stateMap.insert(std::make_pair(slmState, topstates));
        _pushScoreHeap(state.m_score, slmState);
    } else {
        inserted = it->second.push(state);
        slmState = it->second.top().m_slmState;
        _adjustDown(m_heapIdx[slmState]);
    }
    if (inserted) m_size++;

    if (m_size > beam_width) {
        slmState = m_scoreHeap[0].second;
        it = m_stateMap.find(slmState);

        // pop one node from it, and if it's empty, remove it from map and heap
        it->second.pop();
        if (it->second.size() == 0) {
            m_stateMap.erase(it);
            _popScoreHeap();
        } else {
            m_scoreHeap[0].first = it->second.top().m_score;
            _adjustDown(0);
        }
        m_size--;
    }
}

void
CLatticeStates::_pushScoreHeap(TSentenceScore score, CSlmState slmState)
{
    m_scoreHeap.push_back(std::make_pair(score, slmState));
    _adjustUp(m_scoreHeap.size() - 1);
}

void
CLatticeStates::_popScoreHeap()
{
    m_heapIdx.erase(m_scoreHeap[0].second);
    m_scoreHeap[0] = m_scoreHeap[m_scoreHeap.size() - 1];
    m_scoreHeap.pop_back();
    if (m_scoreHeap.size() > 0) {
        _refreshHeapIdx(0);
        _adjustDown(0);
    }
}

void
CLatticeStates::_refreshHeapIdx(int heapIdx)
{
    m_heapIdx[m_scoreHeap[heapIdx].second] = heapIdx;
}

void
CLatticeStates::_adjustUp(int node)
{
    int parent = (node - 1) / 2;
    while (parent >= 0) {
        if (m_scoreHeap[parent].first < m_scoreHeap[node].first) {
            std::swap(m_scoreHeap[parent], m_scoreHeap[node]);
            _refreshHeapIdx(parent);
            node = parent;
            parent = (node - 1) / 2;
        } else {
            _refreshHeapIdx(node);
            return;
        }
    }
}

void
CLatticeStates::_adjustDown(int node)
{
    int left = node * 2 + 1;
    int right = node * 2 + 2;
    while (left < (int) m_scoreHeap.size()) {
        int child = -1;
        if (m_scoreHeap[node].first < m_scoreHeap[left].first) {
            child = left;
        } else if (right < (int) m_scoreHeap.size()
                   && m_scoreHeap[node].first < m_scoreHeap[right].first) {
            child = right;
        } else {
            _refreshHeapIdx(node);
            return;
        }
        std::swap(m_scoreHeap[node], m_scoreHeap[child]);
        _refreshHeapIdx(child);
        node = child;
        left = node * 2 + 1;
        right = node * 2 + 2;
    }
}

CLatticeStates::iterator
CLatticeStates::begin()
{
    CLatticeStates::iterator it;
    it.m_mainIt = m_stateMap.begin();
    it.m_mainEnd = m_stateMap.end();
    it.m_childIt = it.m_mainIt->second.begin();
    return it;
}

CLatticeStates::iterator
CLatticeStates::end()
{
    CLatticeStates::iterator it;
    it.m_mainEnd = it.m_mainIt = m_stateMap.end();
    return it;
}

void
CLatticeStates::iterator::operator++()
{
    ++m_childIt;
    if (m_childIt == m_mainIt->second.end()) {
        ++m_mainIt;
        if (m_mainIt != m_mainEnd)
            m_childIt = m_mainIt->second.begin();
    }
}

bool
CLatticeStates::iterator::operator!=(const CLatticeStates::iterator& rhs)
{
    if (m_mainIt == m_mainEnd || rhs.m_mainIt == rhs.m_mainEnd) {
        return m_mainIt != rhs.m_mainIt;
    } else {
        return m_mainIt != rhs.m_mainIt && m_childIt != rhs.m_childIt;
    }
}

TLatticeState&
CLatticeStates::iterator::operator*()
{
    return m_childIt.operator*();
}

TLatticeState*
CLatticeStates::iterator::operator->()
{
    return m_childIt.operator->();
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
